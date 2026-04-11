Attention quand on lance le programme, ping doit lancer un paquet "originel" a la seconde 0, ce qui veux dire que si on utilise l'option -w, on doit avoir le temps d'envoyer 6 paquet

/!\ Attention il faut lancer le programme en super user (penser a ajouter la vérif)
/!\ Attention a gerer comment ća marche quand -c et -w sont en meme temps


-c s'arrete au bout de X paquet envoyé
-i interval entre deux paquet
-w nombre de secondes au bout du quel le programme s'arrete
-s taille du paquet a envoyer
--ttl nombre de "relai" que peut croiser le paquet



A test 

ping ---vvvv
ping ---w
ping -s 10
ping -i 0.2
ping -i 0.2.
ping -i 0.
ping -i 1
ping -i 0..
ping -i 0..0
ping -s 0..0
ping -ttl 0
ping -ttl 1
ping -ttl 255
ping -ttl 256
ping ""
ping -s ""
ping -i ""
ping -c -w