# Llamadas para consulta de acceso a usuarios

El sistema cuenta con una REST API en la nube que recibe el ID de los usuarios y responde si puede accesar o no. El URL de producción es 

`http://142.93.93.25/api/Students/canPass?nfcId=<userID>`

El argumento *userID* es un string de 8 caracteres alfanumérico, por ejemplo *199059D3*

Por lo que una llamada ejemplo sería:

`http://142.93.93.25/api/Students/canPass?nfcId=199059D3`

La respuesta es un **string** con dos opciones; *"true"* o *"false"*



## Código de apoyo para desarrollo

Para fines de desarrollo se tiene un código que levanta una REST API de manera local en esta [ruta](https://github.com/edmundormz/aquatica/blob/master/microserver/hello.py). Este código es sólo de referencia, puede o no ser usado para desarrollo final.

El hardware lector extrae el ID de las tarjetas y lo envía a través de una llamada GET al servidor local (raspberry), corriendo  bajo el siguiente formato:

`http://<raspberry ip>/checkStringID/<user ID>`

Por ejemplo:

`http://192.168.100.106/checkStringID/81007408`

Y se recibe como respuesta un string, *"true"* o *"false"*



