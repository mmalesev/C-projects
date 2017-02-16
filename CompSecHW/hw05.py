class RC4:
    def __init__(self, user_key):
        if(len(user_key)==16 and type(user_key)==str):
            self.key = user_key.lower()
        else:
            default_key = "iajskdueirtionhs"
            self.key = default_key
            print("Inappropriate key used - key needs to be 16 characters long string. Default key used instead")

    def encrypt(self, originalImage):
        s = self.__generateS(self.key)
        encrypted_list = []
        i = 0
        j = 0
        for data in originalImage:
            i = (i + 1) % 256
            j = (j + s[i]) % 256
            s[i], s[j] = s[j], s[i]
            out_index = (s[i] + s[j]) % 256
            encrypted_list.append(data ^ s[out_index])
        encrypted_out = bytes(encrypted_list)
        return encrypted_out

    def decrypt(self, encryptedImage):
        output = self.encrypt(encryptedImage)
        return output

    def __generateS(self, key):
        key_int = [ord(x) for x in key]
        s = [x for x in range(256)]
        t = []
        for i in range(256):
            t.append(key_int[i % len(key_int)])
        j = 0
        for i in range(256):
            j = (j + s[i] + t[i]) % 256
            s[i], s[j] = s[j], s[i]
        return s

if __name__=="__main__":
    rc4Cipher = RC4("asdfkshi")
    with open("winterTownNoHeader.ppm", 'rb') as f:
        originalImage = f.read()
    encryptedImage = rc4Cipher.encrypt(originalImage)
    decryptedImage = rc4Cipher.decrypt(encryptedImage)
    with open("output.ppm", 'wb') as f:
        f.write(encryptedImage)
    if(decryptedImage == originalImage):
        print("hooray!")
    else:
        print("****")
