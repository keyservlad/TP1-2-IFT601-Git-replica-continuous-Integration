Ala Antabli anta2801
Arnaud Guilhamat guia2612

Pour ce TP nous avons utilisé la bibliothèque yaml-cpp qui est incluse dans l'installation de Buildus

Setup avec cmake :
se placer dans le directory Buildus, créer un directory build et executer cmake et make dans le build

Pour construire une application :
utiliser la commande "./BuildUs path/config.buildus path/to/destination" (config.buildus se trouve dans tests/testsUtils/config.buildus)
--> par exemple : "./BuildUs tests/testsUtils/config.buildus tests/testsUtils"
un executable est créé executable avec la commande "./tests/testsUtils/helloworld"

Pour clean :
utiliser la commande "./BuildUs clean"

Pour effectuer les tests, se placer dans le dossier Buildus :
"./tests/buildtests"