# Huffman-Compression

Projet universitaire en langage **C** implémentant l’algorithme de **compression et décompression Huffman**, réalisé dans le cadre du module *Advanced Data Structures (UIR)*.

L’objectif du programme est de compresser un fichier texte en exploitant la fréquence des caractères, puis de permettre la décompression pour retrouver le fichier original.

---

## 🧠 Principe de l'algorithme de Huffman

L’algorithme de Huffman repose sur l’idée suivante :

- Les **caractères les plus fréquents** doivent avoir des **codes binaires plus courts**.
- Les **caractères rares** auront des codes plus longs.
- L’ensemble forme un **arbre binaire** (Huffman Tree) permettant un encodage optimal.

### Étapes principales :

1. Calcul des **fréquences** de chaque caractère.
2. Construction d’une **min-heap** des nœuds.
3. Fusion progressive des deux nœuds les moins fréquents → création de l’arbre.
4. Génération des **codes binaires**.
5. **Compression** du fichier texte.
6. **Décompression** à partir du fichier `.huf`.

---
## 📁 Structure du projet

ProjetHuffman/
│── heap.c / heap.h → Gestion de la min-heap utilisée pour l’arbre
│── huffman.c / huffman.h → Construction de l’arbre et génération des codes
│── main.c → Point d’entrée du programme
│── output.txt → Fichier généré après décompression
│── testX.txt → Fichiers texte d'entrée (tests)
│── testX.huf → Fichiers compressés obtenus
│── ProjetHuffman.cbp → Fichier de projet Code::Blocks
│── .gitignore → Exclut exécutables et fichiers temporaires


Compiler avec GCC :

```bash
gcc -o huffman main.c huffman.c heap.c


▶️ Utilisation
🔹 Compression
./huffman -c input.txt output.huf

🔹 Décompression
./huffman -d input.huf output.txt

📊 Taux de compression

Selon le contenu du texte, le taux de compression peut varier entre 20% et 70%.
Les caractères très répétitifs compressent beaucoup mieux.

🚀 Améliorations possibles

-Affichage graphique de l’arbre de Huffman
-Interface CLI plus ergonomique
-Mesure automatique du taux de compression
-Support Unicode (UTF-8)
-Interface graphique simple (GTK / WinForms)

👨‍💻 Auteur
Projet réalisé par Ismail Frouij — Université Internationale de Rabat.
