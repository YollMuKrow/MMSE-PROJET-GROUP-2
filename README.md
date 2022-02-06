# MMSE-PROJET-GROUP-2
## Projet en binome UE MMSE
### Alexis Lheritier - Alexandre Massonié

Précision spatial :  256 valeurs possibles \
Précision : 1 point correspond à 1,40 degré d’arc\
Rafraîchissement des leds  :\
Rotation  : 600 000 instructions\
Mémoire utilisé : buffer frame = 32*16 = 512 octets sur la mémoire SRAM\
programme = 2ko (sans bitmap) sur la mémoire FLASH

Précision temporel : \
réel = 0.9996309 secondes

Après 6 mois : \
Attendue : 15 552 000 secondes\
Réel : 15 546 260  secondes\
écart : 1h 35 min 40 secondes

Efficacité : \
interruption -> incrémentation de l’heure = 17 cycles\
Bluetooth -> réception = 5 à 16 cycles
