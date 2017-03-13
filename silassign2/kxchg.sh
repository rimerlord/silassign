#creating public parameters for all to use
openssl genpkey -genparam -algorithm DH -out dhp.pem
#put readable contents into dhp_text.txt
openssl pkeyparam -in dhp.pem -text > dhp_text.txt
#generate public and private keys for the server
openssl genpkey -paramfile dhp.pem -out dhkey_server.pem
#generate public and private keys for the bidder1
openssl genpkey -paramfile dhp.pem -out dhkey_bidder1.pem
#generate public and private keys for the bidder2
openssl genpkey -paramfile dhp.pem -out dhkey_bidder2.pem
#generate public and private keys for the viewer
openssl genpkey -paramfile dhp.pem -out dhkey_viewer.pem
#put readable contents of bidders, viewers and server into respective files
openssl pkey -in dhkey_server.pem -text -noout > dhkey_server.txt
openssl pkey -in dhkey_bidder1.pem -text -noout > dhkey_bidder1.txt
openssl pkey -in dhkey_bidder2.pem -text -noout > dhkey_bidder2.txt
openssl pkey -in dhkey_viewer.pem -text -noout > dhkey_viewer.txt
#generate distributable public keys for all 4 nodes
openssl pkey -in dhkey_server.pem -pubout -out dhkey_server_pub.pem
openssl pkey -in dhkey_bidder1.pem -pubout -out dhkey_bidder1_pub.pem
openssl pkey -in dhkey_bidder2.pem -pubout -out dhkey_bidder2_pub.pem
openssl pkey -in dhkey_viewer.pem -pubout -out dhkey_viewer_pub.pem
#shared secret for server and bidder1
openssl pkeyutl -derive -inkey dhkey_server.pem -peerkey dhkey_bidder1_pub.pem -out sb1.bin
#shared secret for server and bidder2
openssl pkeyutl -derive -inkey dhkey_server.pem -peerkey dhkey_bidder2_pub.pem -out sb2.bin
#shared secret for server and viewer
openssl pkeyutl -derive -inkey dhkey_server.pem -peerkey dhkey_viewer_pub.pem -out sv.bin
#create directories if they do not exist
mkdir bidder1
mkdir bidder2
mkdir server
mkdir viewer
#move the shared secrets and key files to directories
mv dhkey_bidder1.pem dhkey_bidder1_pub.pem bidder1
mv dhkey_bidder2.pem dhkey_bidder2_pub.pem bidder2
mv dhkey_server.pem dhkey_server_pub.pem server
mv dhkey_viewer.pem dhkey_viewer_pub.pem viewer
#copy shared secrets into respective folders
cp sb1.bin bidder1
cp sb1.bin server
cp sb2.bin bidder2
cp sb2.bin server
cp sv.bin viewer
cp sv.bin server
#delete shared secrets from outside
rm *.txt *.pem *.bin
