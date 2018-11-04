#Remember to: 
# export FLASK_APP=hello.py (this file name)
# To make server externally visible use: --host=0.0.0.0 (flask run --host=0.0.0.0)
# Card ID is 81:00:74:08
# Beacon ID is 19:90:59:D3

from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello_world():
	return "Hello, wordl!"

@app.route("/checkIntID/<int:id>")
def check_int__id(id):
	return "User ID is %d" % id

@app.route("/checkStringID/<id>")
def check_string_id(id):
	if id == "81:00:74:08": #Card
		return "Yes"
	if id == "19:90:59:D3": #Beacon
		return "Maybe"
	return "No"

if __name__ == "__main__":
	app.run(debug=True, host="0.0.0.0", port=80)