## SIL Assignment 2
### Project 4:
The origin of this lies in procuring material after calling for bids against a tender document. The bids are sent to a server which will allow authorized persons to subsequently download and view the same. While the bids are in some standard format, the bid itself must be secured in several ways:   
1. The bid document is digitally signed to authenticate its origin and that it has not been altered,    
2. It is kept confidential but may be viewed by a set of authorized persons, but only after a certain time t2,    
3. The bid is received before time t1.  
* * *
### Relevant questions in this case are:
1. How does one ensure that information is not altered during communication and storage within server?    
2. In what way are digital signatures relevant?   
3. Would access to “public-key certificate” issued by a certification authority be an issue?    

* * *

## Analysis of the problem
* **Making the bid**
	1. Allow making bids only until time t1   
	2. Each bid has to be digitally signed hence create digital signature scheme for all clients
	3. Using a hashing algorithm make the system capable of performing integrity-check.
	
* **Downloading and viewing the bid**
	1. Allow downloading the bid only after time t2
	2. Only authorised clients can download and view it so we can use login scheme.
	3. Store the bids on server after locking it with public key of authorised personnel so that the downloaded files have to be unlocked using private keys of authorised personnel.

* * *
## Design Challenges
* **Time**  
Use clock() from time.h to get the time elapsed since the start of the program. The server refuses any request to make a bid if it accesses time and finds it to be greater than t1. Similarly server refuses to show the bids until time elapsed is not greater than t2.  
* **Digital Signatures**  
Bidders - 2, Server - 1, Authorised personnel - 2.  
Let each have a pub\_key and priv\_key pair.  
The bids are made and signed by priv\_key of the bidders then locked using pub\_key of the server.  
* **Integrity Check**  
Integrity check is automatically implemented here using digital signatures as if someone alters the bit sequence being sent then it wont unlock by the private key of the server.  

* **Authorised access to bids**
The format to make requests shall be such that -  
> identity(unencrypted) ||   digital signature(of the claimed identity)  ||  request(grant me access)  

The server upon receiving this packet will read the identity and see if the person has been authorised access or not. In case of authorised access matching then strips the identity bits and verifies the signature in case it succeeds it means the request is genuine and then returns with the bundle of bids to download which have been digitally signed by the server to be read by the authorised person only. The authorised party unlocks it and reads the bids.
