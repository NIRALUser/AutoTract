#!/usr/bin/env python3

import os
import sys
import signal
import subprocess
import re
import zipfile
import json
import shutil
import argparse
import logging

dockerfile_url = 'https://raw.githubusercontent.com/NIRALUser/Trafic/master/Docker/cpu/Dockerfile'

# docker paths (can be hard-coded)
input_directory_bind_point = '/root/input'
output_directory_bind_point = '/root/output'
extracted_model_directory_bind = output_directory_bind_point + '/model'
docker_trafic_dir = '/Trafic'

# parameters
# @word@ default parameters are to be replaced by AutoTract.
parser = argparse.ArgumentParser()
parser.add_argument('--inputDirectory', action='store', dest='inputDirectory', default=@inputDirectory@)
parser.add_argument('--outputDirectory', action='store', dest='outputDirectory', default=@outputDirectory@)
parser.add_argument('--displacementFieldPath', action='store', dest='displacementFieldPath', default=@displacementFieldPath@)
parser.add_argument('--checkpointArchivePath', action='store', dest='checkpointArchivePath', default=@checkpointArchivePath@)
parser.add_argument('--log', action='store', dest='log', default=@log@)
parser.add_argument('--generateTracts', action='store', type=bool, dest='generateTracts', default=@generateTracts@)
parser.add_argument('--input_list', nargs='+', dest='inputList', default=@inputList@)
parser.add_argument('--dockerImage', action='store', dest='dockerImage', default='trafic:trafic_cpu')
parser.add_argument('--traficDir', action='store', dest='traficDir', default=@traficDir@)
parser.add_argument('--enableDocker', action='store', default=@enableDocker@)


container = None
# the low-level API is used for building & exec, as it's the only way to get output stream
CLI = None

def stop(signal, frame):
    LOGGER.info('*************** Signal stop received! ******************')

    if container is not None:
        container.remove()
    sys.exit(0)


def initializeLogging(log):
    global LOGGER
    LOGGER = logging.getLogger('Classification')
    LOGGER.setLevel(logging.DEBUG)
    file_handler = logging.FileHandler(log)
    file_handler.setLevel(logging.DEBUG)
    file_formatter = logging.Formatter('%(message)s')
    file_handler.setFormatter(file_formatter)
    LOGGER.addHandler(file_handler)


def escape_ansi(line): #for logging purposes
    ansi_escape = re.compile(r'(\x9B|\x1B\[)[0-?]*[ -/]*[@-~]')
    return ansi_escape.sub('', line)


def checkFileExistence(fileName):
    try:
        with open(fileName):
            return True
    except IOError:
        return False


def get_model_parameters(extracted_model_description_path):
    with open(extracted_model_description_path) as json_desc_file:
        json_string = json_desc_file.read()
        dictionary = json.loads(json_string)
        return dictionary['store_parameters']


def extract_model(checkpoint_archive_path, extracted_model_directory):
    with zipfile.ZipFile(checkpoint_archive_path) as zip_ref:
        zip_ref.extractall(extracted_model_directory)


def build_docker_image(name, client, dockerfile):
    try:
        #Downloading dockerfile
        LOGGER.info('Downloading dockerfile from: ')
        LOGGER.info(dockerfile_url)
        with open(dockerfile, 'w+') as dockerfile_handle:
            req = requests.get(dockerfile_url)
            dockerfile_handle=req.text
            dockerfile_handle = BytesIO(dockerfile_handle.encode('utf-8'))
            for line in CLI.build(fileobj=dockerfile_handle, tag=name, nocache=True, forcerm=True):

                LOGGER.info(escape_ansi(str(json.loads(line)['stream'])))
        return client.images.get(name)
    except requests.exceptions.RequestException as exception:
        LOGGER.error('Request exception while fetching Dockerfile !')
        LOGGER.error(exception)
        return None
    except docker.errors.BuildError as exception:
        LOGGER.error(exception.msg)
        LOGGER.error('Failed to build the Docker image.')
    return None     


def get_docker_image(name, client, dockerfile):
    try:
        image = client.images.get(name)
    except docker.errors.ImageNotFound:
        LOGGER.info(name + ' Docker image not found. Building it now...')
        image = build_docker_image(name, client, dockerfile)
    except docker.errors.APIError as exception:
        LOGGER.error(exception.response.reason)
        LOGGER.error('Docker API error, please check your docker installation')
        return None
    return image


def get_running_docker_container(name, client, image, input_directory, output_directory):
    try:
        container = client.containers.get(name)
    except docker.errors.NotFound:
        LOGGER.info('container not found, creating it now...')
        container = client.containers.create(
            image,
            volumes={output_directory: {'bind': output_directory_bind_point, 'mode': 'rw'},
                     input_directory: {'bind': input_directory_bind_point, 'mode': 'rw'}})
    except docker.errors.APIError as exception:
        LOGGER.error(exception.response.reason)
        LOGGER.error('Docker API error, please check your docker installation')
        return None
    container.start()
    return container


def run_docker_command(command, container):
    LOGGER.info('Running command on docker:\n' + subprocess.list2cmdline(command))
    try:
        
        exec_id = CLI.exec_create(container.name, subprocess.list2cmdline(command))
        for line in CLI.exec_start(exec_id, stream=True, detach=False):
            LOGGER.info(escape_ansi(line.decode()))
    except Exception as exception:
        LOGGER.error(type(exception))
        LOGGER.error(exception)

    LOGGER.info('Command finished.')


def generate_input_csv(input_list, output_directory, extracted_landmarks_path, displacement_field_bind_path):
    with open(output_directory + '/input.csv', 'w') as csv_input_file:
        for input_file in input_list:
            fiber_name = os.path.basename(os.path.dirname(input_file))
            csv_input_file.write(
                input_file + ',' + os.path.join(output_directory, fiber_name) + ','
                + os.path.join(output_directory, 'model', 'model') + ','
                + os.path.join(output_directory, 'model', 'summary') + ','
                + displacement_field_bind_path + ','
                + extracted_landmarks_path + '\n')


def run():

    signal.signal(signal.SIGINT, stop)
    signal.signal(signal.SIGTERM, stop)

    args = parser.parse_args() 

    enableDocker = args.enableDocker

    initializeLogging(args.log)

    if(enableDocker):
        try:
            import docker
            import requests
        except ImportError:
            LOGGER.error("Impor error for docker or requests library. Try running natively or make sure to install the packages in python.")
            sys.exit(-1)

        CLI = docker.APIClient(timeout=10000000) 
        run_docker(args)
    else:
        run_native(args)

def run_native(args):
    input_directory = args.inputDirectory
    output_directory = args.outputDirectory
    displacement_field_path = args.displacementFieldPath
    checkpoint_archive_path = args.checkpointArchivePath
    extracted_model_directory = os.path.join(output_directory, 'model')
    extracted_landmarks_path = os.path.join(extracted_model_directory, 'landmarks.fcsv')
    extracted_model_description_path = os.path.join(extracted_model_directory, 'model', 'dataset_description.json')
    dockerImage=args.dockerImage

    LOGGER.info('Extracting model information')
    extract_model(checkpoint_archive_path, extracted_model_directory)

    input_list = []
    for i in filter(None, args.inputList):
        input_list.append(os.path.join(input_directory, i))

    LOGGER.info('Generating input csv')
    generate_input_csv(input_list=input_list,
                       output_directory=output_directory,
                       extracted_landmarks_path=extracted_landmarks_path,
                       displacement_field_bind_path=displacement_field_path)

    LOGGER.info('Running classification natively')
    arguments = [sys.executable, '-u',
                 os.path.join(args.traficDir, 'TraficMulti/TraficMulti_cli.py'),
                 '--input_csv', os.path.join(output_directory , 'input.csv')]

    execute(arguments)

    if args.generateTracts:
        LOGGER.info('Classification finished. extracting fibers')
        for i in input_list:
            fiber_name = os.path.basename(os.path.dirname(i))
            arguments = [sys.executable, '-u',
                         os.path.join(args.traficDir, 'TraficLib/extractClassifiedFibers.py'),
                         '--class_data',
                         os.path.join(output_directory,
                                      fiber_name,
                                      'classification_output.json'),
                         '--input', i, '--output_dir',
                         os.path.join(output_directory, fiber_name, 'extracted_fibers')]

            execute(arguments)

def run_docker(args):
    ## Parameters
    input_directory = args.inputDirectory
    output_directory = args.outputDirectory
    displacement_field_path = args.displacementFieldPath
    checkpoint_archive_path = args.checkpointArchivePath
    extracted_model_directory = os.path.join(output_directory, 'model')
    extracted_landmarks_path = os.path.join(extracted_model_directory_bind, 'landmarks.fcsv')
    extracted_model_description_path = os.path.join(extracted_model_directory, 'model', 'dataset_description.json')

    dockerImage=args.dockerImage

    dockerfile_local_path = os.path.join(output_directory, 'Dockerfile')
    displacement_field_bind_path = os.path.join(output_directory_bind_point,
                                                os.path.basename(displacement_field_path))

    arg_input_list = filter(None, args.inputList)

    input_list = []
    for i in arg_input_list:
        input_list.append(os.path.join(input_directory_bind_point, i))

    signal.signal(signal.SIGINT, stop)
    signal.signal(signal.SIGTERM, stop)

    # Copying the dField inside the output dir (so docker can access it)
    shutil.copyfile(displacement_field_path,
                    os.path.join(output_directory, os.path.basename(displacement_field_path)))

    LOGGER.info(args)
    LOGGER.info(dockerfile_local_path)
    LOGGER.info('=== Classification ===')

    LOGGER.info('Checking trafic installation on docker')
    client = docker.from_env()
    if client is None:
        LOGGER.error('No docker client found.. Exiting')
        stop()


    if dockerImage == 'trafic:trafic_gpu':
        global dockerfile_url
        dockerfile_url = 'https://raw.githubusercontent.com/NIRALUser/Trafic/master/Docker/gpu/Dockerfile'

    LOGGER.info('Getting docker image')
    image = get_docker_image(name=dockerImage, client=client,
                             dockerfile=dockerfile_local_path)
    if image is None:
        LOGGER.error('Unable to find or build docker image. Exiting classification')
        stop()

    LOGGER.info('Getting docker container')
    container = get_running_docker_container(name='trafic_container_autotract', client=client,
                                             image=image, input_directory=input_directory,
                                             output_directory=output_directory)
    

    if container is None:
        LOGGER.error('Unable to find or build docker container. Exiting classification')
        stop()

    LOGGER.info('Generating input csv')
    generate_input_csv(input_list=input_list,
                       output_directory=output_directory,
                       extracted_landmarks_path=extracted_landmarks_path,
                       displacement_field_bind_path=displacement_field_bind_path)

    LOGGER.info('Extracting model information')
    extract_model(checkpoint_archive_path, extracted_model_directory)
    # get_model_parameters(extracted_model_description_path)

    LOGGER.info('Running classification on docker')
    arguments = ['python', '-u',
                 os.path.join(docker_trafic_dir, 'TraficMulti/TraficMulti_cli.py'),
                 '--input_csv', os.path.join(output_directory_bind_point , 'input.csv')]

    run_docker_command(arguments, container)

    if args.generateTracts:
        LOGGER.info('Classification finished. extracting fibers')
        for i in input_list:
            fiber_name = os.path.basename(os.path.dirname(i))
            arguments = ['python', '-u',
                         os.path.join(docker_trafic_dir, 'TraficLib/extractClassifiedFibers.py'),
                         '--class_data',
                         os.path.join(output_directory_bind_point,
                                      fiber_name,
                                      'classification_output.json'),
                         '--input', i, '--output_dir',
                         os.path.join(output_directory_bind_point, fiber_name, 'extracted_fibers')]

            run_docker_command(arguments, container)

    LOGGER.info('Shutting container down')
    container.stop()
    LOGGER.info('Removing container')
    container.remove()
    LOGGER.info('Done')

def execute(args):
    
    LOGGER.debug(subprocess.list2cmdline(args))
    stdout, stderr = subprocess.Popen(args).communicate()
    LOGGER.debug(stdout)
    LOGGER.error(stderr)

    return stdout


if __name__=="__main__":
    run()

