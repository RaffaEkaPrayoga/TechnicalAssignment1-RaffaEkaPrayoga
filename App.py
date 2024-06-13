from flask import Flask, request, Response
import json

app = Flask(__name__)

temp_list = []

@app.route('/sensor/data', methods=["POST", "GET"])
def sensor():
    if request.method == 'POST':
        try:
            data = request.get_json()
            temperature = data["temperature"]
            humidity = data["humidity"]
            timestamp = data["timestamp"]

            temp_list.append({
                "temperature": temperature,
                "humidity": humidity,
                "timestamp": timestamp
            })

            response_data = {
                'message': 'Data saved successfully'
            }

            response = Response(
                json.dumps(response_data),
                status=200,
                mimetype='application/json'
            )

            return response

        except Exception as e:
            response_data = {
                'message': 'Failed to process request',
                'error': str(e)
            }
            response = Response(
                json.dumps(response_data),
                status=400,
                mimetype='application/json'
            )
            return response
    else:
        response_data = {
            'temperature_list': temp_list
        }
        response = Response(
            json.dumps(response_data),
            status=200,
            mimetype='application/json'
        )
        return response

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
