# Steganographic Encoder/Decoder
A program which can encode/decode any file from a .bmp image (optionally uses encryption via a password for even more security) 

## What is Steganography ?
Steganography  is  the  practice  of  hiding  sensitive  data  in  plain  sight.   This is a tool that can conceal any type of file or message within a .BMP image,  whilst ensuring the new altered image is essentially indistinguishable from the original. 

## Install (Linux only for now)
    
    sudo apt-get install gcc make git -y
    git clone https://github.com/FeLiNa22/steganography.git 
    cd steganography
    make

## How to use
Can be run using the following terminal commands (Note : Anything in brackets is optional):  
    
    ./steg encode <input *.bmp> <file to encode> (-o <output path *.bmp>) (-p <password to encrypt data>)  
    ./steg decode <input *.bmp> (-p <password>)

NOTE: if no password is provided, the "file to encode" will not be encrypted and hence will not as secure


## Implementation
The encoding works as every pixel of the image requires almost 32 bits each and changing the least significant bits of these pixels is unnoticeable by the human eye. We take advantage of this fact to encode as every single pixel with each bit of the file being encoded. 

An example is provided 'example/shakespeare.bmp'.

![shakespeare.bmp](https://github.com/FeLiNa22/steganography/blob/main/example/shakespeare.bmp)

This image has been encoded with the entire works of Shakespeare (in the form of a .txt file) and all the data is encrypted using the password 'romeo'.

*Don't believe me*

Run 
   
    ./steg decode example/shakespeare.bmp -p romeo
    
and you will find a file 'full_shakespeare.txt' has appeared.
 
To enhance the security of the encoded image we also added the optional password to encrypt the data, before it is even encoded into the image.
This prevents anyone who manages to steal the image from actually decoding the senssitive data.

This  password  can  be  optionally  specified  using  the  flag '-p' <password>,  and  this  password  is  converted  into  the starting key for the encryption process using a hashing algorithm.

For a more detailed description of the encryption process, design implementations and testing, see the 'Extension' section of the 'Report.pdf' file.

## Use Cases

If you are trying to send sensitive data through a public medium without anyone noticing then this is the perfect solution for you.

As we basically are hiding data in plain sight you could potentially upload an encoded image onto any social media (we tested instagram) and then anyone who has been provided with the original password to the encoded image can then save the image, decode it using my program and now can read the encoded file.
The same works for sending images over Whatsapp and any other social media.  
