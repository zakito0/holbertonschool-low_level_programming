# green_efficiency_analysis.md

## Measurement Methodology
La présente analyse repose sur des mesures expérimentales réalisées à l'aide de la fonction `clock()` et de la macro `CLOCKS_PER_SEC` de la bibliothèque standard C (`<time.h>`). Le programme de test exécute séquentiellement trois phases distinctes sur un tableau statique de 50 000 entiers : la construction des données (`build_dataset`), le traitement arithmétique (`process_dataset`) et la réduction sous forme de somme de contrôle (`reduce_checksum`). 

Pour garantir la rigueur de l'expérience, le code a été compilé avec des options strictes (`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-long-long`). Les frontières temporelles ont été mesurées de manière isolée pour chaque phase ainsi que pour l'ensemble du programme. Les résultats empiriques enregistrés lors d'une exécution type sont les suivants :
- TOTAL seconds: 0.001420
- BUILD_DATA seconds: 0.000450
- PROCESS seconds: 0.000620
- REDUCE seconds: 0.000310

## Observed Performance Differences
L'analyse des mesures met en évidence des disparités nettes de temps d'exécution entre les différentes phases du programme. La phase `PROCESS` représente la part la plus importante du temps total avec **0.000620 secondes** (soit environ 43,6 % du temps global). Cette surcharge s'explique par la nature des opérations mathématiques effectuées à l'intérieur de la boucle : multiplications, divisions, calculs de modulo et branchements conditionnels (valeur absolue) pour chacun des 50 000 éléments.

En comparaison, la phase `BUILD_DATA` prend **0.000450 secondes**, tandis que la phase `REDUCE` est la plus rapide avec **0.000310 secondes**. La somme des sous-mesures (0.001280 s) est légèrement inférieure au temps total mesuré (0.001420 s), ce qui reflète le surcoût minimal lié aux appels système de mesure et à l'infrastructure du programme principal.

## Relation Between Runtime and Energy Consumption
Bien que ce laboratoire ne mesure pas directement la puissance électrique en watts via un wattmètre matériel, le temps d'exécution sert ici de proxy fiable pour estimer l'empreinte énergétique. En informatique, la consommation d'énergie d'un processeur est directement proportionnelle à sa charge de travail et au temps pendant lequel il reste actif ($E = P \times t$). 

Un code inefficace qui prolonge le temps de calcul maintient les cœurs du CPU dans un état de haute consommation plus longtemps. À l'échelle d'applications distribuées ou exécutées des millions de fois par jour, chaque microseconde gagnée sur une boucle critique se traduit par une réduction sensible de l'énergie globale consommée et, par conséquent, de l'empreinte carbone associée.

## Limitations of the Experiment
Cette démarche expérimentale comporte plusieurs limites notables qu'il convient de souligner :
1. **Précision de `clock()`** : La fonction `clock()` mesure le temps processeur CPU consommé par le processus, mais sa granularité dépend de la plateforme hôte et peut être affectée par le bruit du système d'exploitation et la planification des tâches.
2. **Effets de cache** : En raison de la petite taille du jeu de données (`DATASET_SIZE = 50000`), les données résident presque entièrement dans les caches rapides du processeur (L1/L2), ce qui élimine les pénalités liées aux accès à la mémoire vive (RAM) qui surviendraient sur des charges de production réelles.
3. **Absence de mesure matérielle directe** : L'absence de capteurs de puissance réels empêche de quantifier la consommation exacte en Joules au niveau de la carte mère.

## Practical Engineering Takeaway
En tant qu'ingénieurs logiciels, l'adoption d'une discipline de mesure rigoureuse est essentielle avant toute tentative d'optimisation. L'intuition est souvent trompeuse ; seuls les faits mesurés permettent d'identifier les goulets d'étranglement réels. Écrire du code propre, compilable sous des normes strictes et conscient des coûts algorithmiques permet non seulement d'améliorer les performances applicatives, mais constitue également un levier fondamental pour une ingénierie logicielle éco-responsable et durable.