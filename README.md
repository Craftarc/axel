# Axel Backend
This repository contains the source code for the companion application to the online ARPG, Path of Exile.

Axel is a replacement for Exilence, which has recently gone out of service. This project is in the early stages of development.

# Table of Contents
- [Introduction](#introduction)
- [Overview](#overview)
	- [About Axel](#about-axel)
   	- [Technology Stack](#technology-stack)
   		- [Libraries](#libraries)
   	   	- [Tools](#tools)
   	   	- [Services](#services)
   	- [About AWS Lambda](#about-aws-lambda)
   	  	- [The Lambda Execution Environment](#the-lambda-execution-environment)
- [Setup](#setup)
	- [Cloning the repository](#1-cloning-the-repository)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

# Introduction
This documentation details the architecture of Axel Backend, and guides the user in setting up a development environment. <br>
To contribute to this project, see [Overview](#overview) for a general overview of the project. <br>
To start from scratch, see [Setup](#setup)

# Overview
## About Axel
Axel follows a frontend-backend architecture.

**The backend is an AWS Lambda function**. An Lambda function is code deployed on a server that only activates when the function's endpoints are hit. <br>
More simply put, when the frontend calls the backend with some data, the Lambda function activates, processes the data, and outputs the response back to the
frontend. All code in this repository deals with is what happens between that input and that output.

**The frontend client is a Neutralino.js GUI**, and is generally responsible for: 
- Rendering graphs
- Displaying information about the user's characters, inventories, and net worth

---

To that end, there are **two** main events that require communication between these two components.
1. **On first installation, initiate an Oauth process**. The backend server mediates the process - we follow the standard Oauth Authorization Code Flow.
   - Axel is considered a [Confidential Client](https://www.pathofexile.com/developer/docs/authorization#clients). The redirect URL is the Lambda function URL.
   - The backend manages access tokens and session tokens,
   		- This minimises security implications as the access token is never exposed on the frontend client.
2. **When the frontend client requires new data to update its displayed information _(e.g. chart data, updated currency prices)_, it makes a request to the backend**. The backend is
   responsible for making the appropriate requests to PoE's resource server and sending the data back to the frontend.
   
## Technology Stack
The Axel backend depends on the following technologies. Explicit installation or building of these items _(except Docker)_ is not required, as they are managed within a Docker image.

### Libraries
The Axel backend uses C++17.
1. [Boost](https://www.boost.org/)
2. [Botan](https://botan.randombit.net/)
3. [Google Test](https://github.com/google/googletest)
4. [AWS C++ SDK](https://aws.amazon.com/sdk-for-cpp/)
5. [AWS C++ Runtime](https://github.com/awslabs/aws-lambda-cpp)

### Tools
1. [Docker](https://www.docker.com/)
2. C++17
3. CMake
4. gcc

### Services
1. AWS Lambda
2. AWS DynamoDB
3. PoE Ninja API
4. [Official PoE API](https://www.pathofexile.com/developer/docs/index)

## About AWS Lambda
### The Lambda Execution Environment
When a HTTP request from the Axel frontend client reaches the Lambda function's URL, the request data is stored. The AWS Lambda service then activates the Lambda function, which consists of two layers: The _AWS C++ Runtime_, and our code. The _Runtime_ polls the Lambda service, retrieves the stored request data, then passes it to our code for processing. Finally, our code returns a result to the _Runtime_, which then passes the response back to the Lambda service to be sent back out to the requester.

The consequence of this is that we have two options:

**1. Test our code by hosting our own "mock Lambda service".**
   - `axel/main.cpp` loads the _Runtime_ layer as well as our code. The _Runtime_ will continuously poll a hardcoded endpoint, thinking that it is running inside a real Lambda function, and try to get event data from the Lambda service. Obviously, this will fail, because the code is actually running in a local container. There is no Lambda service.
   - To remedy this, we download the [AWS Runtime Interface Emulator](https://github.com/aws/aws-lambda-runtime-interface-emulator) (RIE). When the emulator binary is run, it acts like the Lambda service, storing event data we pass to it and allowing the _Runtime_ to poll it.
   - To test Axel's backend while simulating the production environment with the AWS Runtime Interface Emulator, see [Working with the RIE](#working-with-the-rie)
     
**2. Test our code independently of the _Runtime_.**
   - In many situations during development, there's no real need to host the RIE, start up the _Runtime_, and exercise paths through our code by _POST_ ing data to the RIE.
   - A simpler solution would be to use `axel/sandbox.cpp` to define custom entrypoints into the libraries and functions of Axel's backend.
   - To test Axel's backend through custom entrypoints, see [Working with custom entrypoints](#working-with-custom-entrypoints)

# Setup
This section guides you through a basic setup of the development environment, concluding with successfully running a Google Test test suite.

**You are strongly recommended to use an IDE like CLion or Visual Studio**. This project assumes the use of CLion.

## 1. Cloning the repository
To clone the repository, `cd` into the directory in which you want the **contents** of the root folder of the repsitory to reside.

The following commands clone the repository into `~/axel/<contents-of-root-repository>`.
```
cd ~
mkdir axel
cd axel
git clone git@github.com:Craftarc/axel.git
```

## 2. Build the Docker image
Axel's development environment is containerised. The _Dockerfile_ may be located at `axel/Dockerfile`.

Run the following command to build the image. <br>
`docker build -t axel-backend:1.0`

_Warning: This process may take 1-2 hours, since several large packages of source code will be downloaded._

## 3. Build the project
Axel's backend builds two main executables: `main.cpp` and `sandbox.cpp`. For more information on why there are two executables, see [The Lambda Execution Environment](#the-lambda-execution-environment).

### CLion
**1. In _Settings_, navigate to _Build, Execution, Deployment > Toolchains_.** <br>
Set the following settings:
- Container settings: `--entrypoint= --rm`
- CMake: `/usr/bin/cmake3`

**2. Navigate to _Run > Edit Configurations... > + > Google Test_.** <br>
Select one of the test targets _(*\_tests)_, then click _Apply_. <br>
Run the configuration.

### Manual Build
**1. Mount the project root into the container, then start the container.** <br>
The following command starts a shell inside the container (overriding the default entrypoint, which asks for an event-data-handling executable).
```
docker run -v <path-to-axel-root>:/app/axel --it --entrypoint /bin/bash axel-backend:1.0
```
**2. Compile and build the executables.** <br>
```
cd build
cmake3 ..
make
```
**3. Run a test.** <br>
```
cd src/auth
./auth_tests
```

# Configuration
## Working with the RIE
The `main` executable starts the AWS C++ runtime, which starts polling the hardcoded Lambda service endpoint to get events. We need to mount the RIE before main is run so there _is_ an endpoint to poll in the first place.
1. [Download](https://github.com/awslabs/aws-lambda-cpp) the RIE for x86_64 platforms.
2. Run the following command:
```
docker build -t axel-backend:1.0 . # Build updated executables
docker run -d -v ~/.aws-lambda-rie:/aws-lambda -p 9000:8080 --entrypoint /aws-lambda/aws-lambda-rie axel-backend:1.0 /app/axel/build/main
```
- `-p 9000:8080`: The RIE listens on port 8080 within the container. By mapping port 9000 on the host machine to port 8080 within the container, event data can be _POST_ed to the RIE.
To _POST_ an event to the RIE, do
```
curl --location 'http://localhost:9000/2015-03-31/functions/function/invocations' \
--header 'Content-Type: application/json' \
--data '{
    "key1": "value1",
    "key2": "value2"
}
```

## Working with custom entrypoints
**1. To work with custom entrypoints, we want to execute the `sandbox` executable instead.** <br>
Simply override the default entrypoint (which tries to find and run an executable that polls the Lambda service).
```
docker build -t axel-backend:1.0 . # Build updated executables
docker run -it --entrypoint /bin/bash
```
**2. Run the `sandbox` executable.**
```
cd build
./sandbox
```

# Contributing

# License


