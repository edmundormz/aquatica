import pymongo

maxSevSelDelay = 3 # connection delay in milliseconds

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
