from flask import Flask, request, jsonify
from data_handling import handle_data
from door import Door

app = Flask(__name__)
d= Door()

@app.route('/get')
def return_data():
    # add return data here
    ret = {"state": f"{d.get_state()}"}
    return '<h1>Hello World! This is my first Python Website.</h1>'


@app.route('/post', methods=['POST'])
def recv_data() -> tuple[dict, int]:
    data = request.get_json()
    if data is None:
        return jsonify({"error": "Invalid JSON"}), 400
    try:
        print(data)
        handle_data(data, d)
        ret = {"message": "Request successful", "status": "ok"}
        return jsonify(ret), 500
    except Exception as err:
        return jsonify({"error": str(err)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)