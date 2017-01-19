import string

def encrypt_char(character, key):
    #case for the non-alphabetic character which we do not want to change

    if (character.isupper()):
        result = (ord(character) - 65 + ord(key) - 97) % 26
        result = chr(result + 65)
    else:
        result = (ord(character) - 97 + ord(key) - 97) % 26
        result = chr(result + 97)
    return result

with open("input.txt") as f:
    input = f.read()
input_list = list(input) #creating list of characters

#applying the same rules for the file containing the encryption key
with open("key.txt") as f:
    key = f.read()
    key = key.lower() #make all letters lowercase
key_list = list(key)
#getting rid of all non-alphabetic characters in the key
i=0
while i < len(key_list):
    if(not key_list[i].isalpha()):
        key_list.pop(i)
        i-=1
    i+=1

output_list = []
key_index = 0 #tracking the index of the element in the key used for encryption
for c in input_list:
    #creating the list of encrypted characters
    if (c.isalpha()):
        output_list.append(encrypt_char(c, key_list[key_index % len(key_list)]))
        key_index += 1
    else:
        output_list.append(c) #do not want to encrypt non-alphabetic characters

outputfile = open('output.txt', 'w')
for c in output_list:
    outputfile.write("%s" % c)

