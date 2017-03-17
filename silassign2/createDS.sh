#Create a certificate request (CSR) file. The "subject" (-subj) describes the user of the certificate. Enter dummy values as shown. The following command line sets the password for the certificate to "abcdefg"
openssl req -passout pass:abcdefg -subj "/C=IN/ST=DL/L=NEWDELHI/O=IIT DELHI/OU=SIL765/CN=Assignment2 CA/emailAddress=ahm.rimer@gmail.com" -new > ca.cert.csr
#Create a key file, ca.key, to store the private key. This removes the password protection from the certificate request file so that you do not have to type the password every time you sign a certificate. Because the password protection has been removed, use the certificate request file for testing purposes only
openssl rsa -passin pass:abcdefg -in privkey.pem -out ca.key
#Create an X.509 digital certificate from the certificate request. The following command line creates a certificate signed with the CA private key. The certificate is valid for 365 days
openssl x509 -in ca.cert.csr -out ca.cert -req -signkey ca.key -days 365
#Create a PKCS#12-encoded file containing the certificate and private key. The following command line sets the password on the P12 file to default. Rational® Performance Tester uses password of default for all PKCS#12 files by default
openssl pkcs12 -passout pass:default -export -nokeys -cacerts -in ca.cert -out ca.cert.p12 -inkey ca.key
#move in CA folder everything except ca.cert and ca.key
mkdir ca
mkdir server
mkdir bidder1
mkdir bidder2
mkdir viewer
mv *.csr *.p12 *.pem ca
#########################################################################
#Create digital certificate for the users
#########################################################################
##Create a CSR file for the users. Set the initial password to their name. Optionally, provide an appropriate subject
#SERVER
openssl req -passout pass:server -subj "/C=IN/ST=DL/L=NewDelhi/O=IIT Delhi/OU=SIL765/CN=Lovely Server/emailAddress=server@server.com" -new > server.cert.csr
#rename privkey.pem to server folder
mv privkey.pem server.cert.csr server
#BIDDER1 and rename and move
openssl req -passout pass:bidder1 -subj "/C=IN/ST=DL/L=NewDelhi/O=IIT Delhi/OU=SIL765/CN=Lovely Server/emailAddress=bidder1@bidder1.com" -new > bidder1.cert.csr
mv privkey.pem bidder1.cert.csr bidder1
#BIDDER2 rename move
openssl req -passout pass:bidder2 -subj "/C=IN/ST=DL/L=NewDelhi/O=IIT Delhi/OU=SIL765/CN=Lovely Server/emailAddress=bidder2@bidder2.com" -new > bidder2.cert.csr
mv privkey.pem bidder2.cert.csr bidder2
#VIEWER
openssl req -passout pass:viewer -subj "/C=IN/ST=DL/L=NewDelhi/O=IIT Delhi/OU=SIL765/CN=Lovely Server/emailAddress=viewer@bidder1.com" -new > viewer.cert.csr
mv privkey.pem viewer.cert.csr viewer
##Create a private key file without a password
openssl rsa -passin pass:server -in server/privkey.pem -out server/server.key
openssl rsa -passin pass:bidder1 -in bidder1/privkey.pem -out bidder1/bidder1.key
openssl rsa -passin pass:bidder2 -in bidder2/privkey.pem -out bidder2/bidder2.key
openssl rsa -passin pass:viewer -in viewer/privkey.pem -out viewer/viewer.key
##Create a new X.509 certificate for the new user, digitally sign it using the user's private key, and certify it using the CA private key. The following command line creates a certificate which is valid for 365 days
openssl x509 -req -in server/server.cert.csr -out server/server.cert -signkey server/server.key -CA ca.cert -CAkey ca.key -CAcreateserial -days 365
openssl x509 -req -in bidder1/bidder1.cert.csr -out bidder1/bidder1.cert -signkey bidder1/bidder1.key -CA ca.cert -CAkey ca.key -CAcreateserial -days 365
openssl x509 -req -in bidder2/bidder2.cert.csr -out bidder2/bidder2.cert -signkey bidder2/bidder2.key -CA ca.cert -CAkey ca.key -CAcreateserial -days 365
openssl x509 -req -in viewer/viewer.cert.csr -out viewer/viewer.cert -signkey viewer/viewer.key -CA ca.cert -CAkey ca.key -CAcreateserial -days 365
##Create a PKCS#12-encoded file. The following command line sets the password on the P12 file to their names
openssl pkcs12 -passout pass:server -export -in server/server.cert -out server/server.cert.p12 -inkey server/server.key
openssl pkcs12 -passout pass:bidder1 -export -in bidder1/bidder1.cert -out bidder1/bidder1.cert.p12 -inkey bidder1/bidder1.key
openssl pkcs12 -passout pass:bidder2 -export -in bidder2/bidder2.cert -out bidder2/bidder2.cert.p12 -inkey bidder2/bidder2.key
openssl pkcs12 -passout pass:viewer -export -in viewer/viewer.cert -out viewer/viewer.cert.p12 -inkey viewer/viewer.key
##Repeat this step to create as many digital certificates as needed for testing. Keep the key files secure, and delete them when they are no longer needed. Do not delete the CA private key file. You need the CA private key file to sign certificates
####################################################################################################
##Create Public keys for all users and save it in public folder which is accessible to all users
####################################################################################################
mkdir public
openssl pkcs12 -passin pass:server -in server/server.cert.p12 -clcerts -nokeys -out public/serverpub.pem
openssl pkcs12 -passin pass:bidder1 -in bidder1/bidder1.cert.p12 -clcerts -nokeys -out public/bidder1pub.pem
openssl pkcs12 -passin pass:bidder2 -in bidder2/bidder2.cert.p12 -clcerts -nokeys -out public/bidder2pub.pem
openssl pkcs12 -passin pass:viewer -in viewer/viewer.cert.p12 -clcerts -nokeys -out public/viewerpub.pem
#create usable public keys
openssl x509 -pubkey -in public/serverpub.pem -noout > public/server.pub.pem
openssl x509 -pubkey -in public/bidder1pub.pem -noout > public/bidder1.pub.pem
openssl x509 -pubkey -in public/bidder2pub.pem -noout > public/bidder2.pub.pem
openssl x509 -pubkey -in public/viewerpub.pem -noout > public/viewer.pub.pem
