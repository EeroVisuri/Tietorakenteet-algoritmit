Käytin tietorakenteina isoa kasaa unordered_map-rakenteita. Päädyin rakenteeseen, koska siihen voi tallentaa tiedon avain - arvo pareina, ja näiden hakeminen, lisääminen ja poisto ovat kaikki keskimäärin vakioaikaisia. Ongelmana tässä on unordered_mapista find-toiminnon käyttäminen, jossa suoritusaika kasvaa logaritmisesti ja hoksasin tämän vasta ajan käydessä vähiin ja toteutuksen ollessa jo melko pitkällä. 

Aakkosjärjestys ja koordinaattien mukainen järjestys ovat molemmat n log n, koska niissä täytyy käyttää std::sort(). Sille kirjotin kaksi vertailufunktiota omituisten ratkaisujeni takia.

Mietin myös structia, johon tunkea kaikki tarvittavat tiedot, mutta päädyin kuitenkin unordered_mappeihin. Structin jämät ovat nähtävissä headerin private-osiossa. 







