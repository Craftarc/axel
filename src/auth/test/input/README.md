## This directory contains JSONs for sample requests obtained from the AWS Lambda Service.

Requests are passed to the main logic by the C++ Runtime as an aws::lambda-runtime::invocation_request.

An invocation_request is a struct of variables. More information about the contents of the struct can be found
in aws/lambda-runtime/runtime.h.

The sample requests in this directory pertain only to the Oauth process.
Each top-level key in the JSON corresponds to a variable name of invocation_request (excluding deadline and
get_time_remaining).

- initial-login: Initial GET request to the Lambda function URL. The function should redirect the user to an
authorisation url, and place a cookie in the user's browser.