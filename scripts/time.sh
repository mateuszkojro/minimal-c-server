curl  -X GET localhost:9090/index.html -w '\nTime: %{time_total}s\n' --output /dev/null
