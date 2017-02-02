from math import sqrt

def isPrime(number):
    for i in range(2, int(sqrt(float(number)))):
        if (int(number) % i == 0):
            return False
    return True

if __name__ == "__main__":
    number = input("Enter the value of n: ")
    if(isPrime(number)):
        output = "field\n"
    else:
        output = "ring\n"

    with open('output.txt', 'w') as out:
        out.write(output)
