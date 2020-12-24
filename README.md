# Steganographic Encoder/Decoder
A program which can encode/decode any file from a .bmp image (optionally uses encryption for even more security) 

## What is Steganography ?
Steganography  is  the  practice  of  hiding  sensitive  data  in  plain  sight.   For  our  extension  we  set  out  to  developing a tool such that we could conceal any type of file within a .BMP image,  whilst ensuring the new altered image is essentially indistinguishable from the original.  To challenge ourselves, we took the approach of only producing legacy code (without the use of libraries).

## Implementation
To enhance the security of the encryption process, we implemented a password encryption process.This  password  can  be  optionally  specified  using  the  flag '-p' <password>,  and  this  password  is  converted  into  the starting key for the encryption process using a hashing algorithm.
  
Our program is able to encode files of literally any data type, because we encode the data into the image at a bit level rather than through grouping data such as words.  

An example is provided in the repository './test/out/shakespeare.bmp'.
This image has been encoded with the entire works of Shakespeare (in the form of a .txt file) and all the data is encrypted using the password 'romeo'.

*Don't believe me*

Run 
   
    './steg decode test/out/shakespeare.bmp -p romeo'
    
and you will find a file 'full_shakespeare.txt' has appeared.
 
 

For a more detailed description of the encryption process, design implementations and testing, see the 'Extension' section of the 'Report.pdf' file.

## How to use
Can be run using the following terminal commands (Note : all flags are optional):  
    
    Encoding: ./steg encode <*.bmp> <file to encode> -o <output path *.bmp> -p <password to encrypt data>  
    Decoding: ./steg decode <*.bmp> -p <password>

NOTE: if no password is provided, the "file to encode" will not be encrypted and hence will not be secure

## Use Cases

If you are trying to send sensitive data without anyone realising, then this is the most perfect solution as it basically hides data in plain sight.
By this it means you could potentially upload an encoded image onto instagram and then anyone whom knows that it is a steganographic image can then retrieve the file from it.
The same works for sending the images over Whatsapp and any other social media.  
