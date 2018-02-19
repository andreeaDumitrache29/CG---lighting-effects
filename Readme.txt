Dumitrache Daniela Andreea
331CB
Elemente de grafica pe calculator - Tema3

	In rezolvarea temei am pornit de la continutul laboratorului 9.
	Scena implementata consta in cele 4 surse de lumina, planul, cerul si santa.
	Sursele de lumina sunt reprezentate prin sfere colorate in culoarea luminii pe care o emit.
Desenarea obiectelor in scena am efectuat-o folosind functia RenderSimpleMesh. Aici se trimit uniformele catre shadere, pentru a se realiza calculele de iluminare.
	Luminile sunt dinamice si au urmatoarele miscari:
-  luminile 1 si 2 : curbe Lissajous
- lumina 3 : curba Talbot
- lumina 4 : curba Plateau
	Miscarea luminilor se poate activa / dezactiva folosind tasta L.

Efectul de tooning:
	Se va desena o a doua instanta de santa, pentru care vor fi vizibile doar fetele spate. Pentru a crea conturul folosind aceasta instanta in Vertex Shader se modifica pozitia vertecsilor, daca este activ efectul de tooning. Pozitia se modifica conform enuntului: pozitia fiecarui vertex se poate modifica prin adunarea unui offset egal cu produsul dintre normala si un factor de scalare (reprezentat de produsul dintre sinus si o constanta). Al doilea santa va fi desenat negru in Fragment Shader.
	Acesta poate fi activat / dezactivat folosind tasta T.

Stepped lighting:
	Efectul de stepped lighting este calculat in Fragment Shader conform indicatiilor din enunt: pentru intensitatile difuza si speculara ale fiecarei lumini se inmulteste valoarea obtinuta cu numLevels, se face floor pe acest produs, iar rezultatul se imparte la numLevels.
	Intensitatile speculara si difuza au fost calculate conform formulelor din laboratorul 8:
- diffuse_light = material_kd * max(dot(world_normal, L), 0);
- specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
	Itensitatea efectului poate fi marita / micsorata folosind tastele M si N.

Texturare si colorare:
	Pentru santa am folosit formula conform enuntului pentru a calcula culoarea finala. Am considerat Ke ca fiind 0 si am ignorat culoarea obiectului. KD si KS au fost extrase din texturile speculara si difuza, iar diffuseFact si specularFact au fost calculati pentru fiecare lumina folosind formulele de mai sus.
Culoarea si pozitiile luminilor sunt primite de Fragment Shader ca uniforme.
Sursele de lumina au drept culoare finala out_color = (light*light_color, 1), unde light = ambient_light + atenuare * (diffuse_light + specular_light);
	Cerul este colorat folosind lumina ambientala.
	Planul este colorat asemanator lui santa, dar neavand culori speculara si difuza KD si KS sunt considerate 1, iar suma intensitatilor ambientala, speculara si difuza se inmulteste cu culoarea luminii.
