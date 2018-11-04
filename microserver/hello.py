#Remember to: 
# export FLASK_APP=hello.py (this file name)
# To make server externally visible use: --host=0.0.0.0 (flask run --host=0.0.0.0)

from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello_world():
	return "Hello, World!"

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0", port=80)