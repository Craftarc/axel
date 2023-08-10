#!/bin/bash

docker run -v ~/.aws-lambda-rie:/aws-lambda \
-p 9000:8080 \
-e AWS_LAMBDA_RUNTIME_API=localhost:9000 \
--entrypoint /aws-lambda/aws-lambda-rie \
axel-backend \
/app/axel/build/main