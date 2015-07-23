import json

with open("prototype_spikingnn.json") as json_file:
    data = json_file.read()

encoded_data = json.loads(data)
print(len(encoded_data))

