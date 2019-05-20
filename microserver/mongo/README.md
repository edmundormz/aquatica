# MongoDB

This guide is focused to `ubuntu OS` in the most recent version so far which is `18.04 LTS (bionic)`

## Installation

1.  Follow the official procedure from [docs.mongodb](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu)

### Enable `mongod` service when system starts

Type the following command:

```bash
$ sudo systemctl enable mongod.service
```

Restart the machine and type the following command to verify that the daemon is correctly enable

```bash
$ sudo service mongod status
● mongod.service - MongoDB Database Server
   Loaded: loaded (/lib/systemd/system/mongod.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2019-05-20 11:08:06 MDT; 1min 8s ago
     Docs: https://docs.mongodb.org/manual
 Main PID: 646 (mongod)
   CGroup: /system.slice/mongod.service
           └─646 /usr/bin/mongod --config /etc/mongod.conf

may 20 11:08:06 Mac systemd[1]: Started MongoDB Database Server.
```

## Create a New Database

1. Type the following command:

```bash
$ mongo
MongoDB shell version v4.0.9
connecting to: mongodb://127.0.0.1:27017/?gssapiServiceName=mongodb
Implicit session: session { "id" : UUID("a0e090f2-3b4e-442f-bd2a-c0e60e4a22ae") }
MongoDB server version: 4.0.9
Server has startup warnings:
2019-05-20T11:08:06.898-0600 I STORAGE  [initandlisten]
2019-05-20T11:08:06.898-0600 I STORAGE  [initandlisten] ** WARNING: Using the XFS filesystem is strongly recommended with the WiredTiger storage engine
2019-05-20T11:08:06.898-0600 I STORAGE  [initandlisten] **          See http://dochub.mongodb.org/core/prodnotes-filesystem
2019-05-20T11:08:08.537-0600 I CONTROL  [initandlisten]
2019-05-20T11:08:08.537-0600 I CONTROL  [initandlisten] ** WARNING: Access control is not enabled for the database.
2019-05-20T11:08:08.537-0600 I CONTROL  [initandlisten] **          Read and write access to data and configuration is unrestricted.
2019-05-20T11:08:08.537-0600 I CONTROL  [initandlisten]
---
Enable MongoDB's free cloud-based monitoring service, which will then receive and display
metrics about your deployment (disk utilization, CPU, operation statistics, etc).

The monitoring data will be available on a MongoDB website with a unique URL accessible to you
and anyone you share the URL with. MongoDB may use this information to make product
improvements and to suggest MongoDB products and deployment options to you.

To enable free monitoring, run the following command: db.enableFreeMonitoring()
To permanently disable this reminder, run the following command: db.disableFreeMonitoring()
---
```

`mongo` shell command allows to enter to the databases

2. Type `use aquatica` which will create a new database in the system
3. Type `db.users.save({userID: "123", authorize: true})` to save a dummy register into `aquatica` local database, otherwise when you exit from mongo the database previouly create wont be saved

## Connecting Services with Python

### Installing Dependencies

```bash
$ sudo apt install python-pip
$ pip install pymongo
```

#### Optional packages

```bash
$ sudo apt install ipython
```

### 

### Connection Script

```python
import pymongo

maxSevSelDelay = 1 # connection delay in milliseconds

try:
    client = pymongo.MongoClient('mongodb://localhost:27017/')
    client.server_info() # force the connection request
except pymongo.errors.ServerSelectionTimeoutError as err:
  print('could not connected to local db because: {}'.format(err))
  
db = client['aquatica']
collection = db['users']

_id = collection.find_one({'userID': 123}, {'_id': 1}) # returns the _id (if any)
dataToInsert = {'userID': 123, 'authorized': True} # data example

if not _id:
  # the record does not exists in db
  collection.insert_one(dataToInsert)
  print('record successfully inserted: {}'.format(dataToInsert))
else:  
  # the record already exists in db
  update = collection.update_one(_id, {'$set': dataToInsert}, upsert=True)
  if (update.modified_count) > 0:
    print('the record: ({}) was successfully updated'.format(dataToInsert))
  else:
    print('the record: ({}) was not changed'.format(dataToInsert))

```

> This script has concurrency allowing to update one by one the existing records in the database and avoiding duplicates