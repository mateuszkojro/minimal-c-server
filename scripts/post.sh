#!/bin/bash
curl http://localhost:8080/organizations/add 
     -X POST
     -H 'Content-Type: application/json' 
     -d '{"foo": "bar"}'
