# webserv

https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

FRENCH 🇫🇷

____________________________________________________________________________________________________________
OSI :
    - Modèle conceptuel. Il s'agit d'une norme permettant à différents systèmes de communiquer entre eux.
      systèmes peuvent communiquer entre eux et utilise 
      7 niveaux d'abstraction.

    4e niveau d'abstraction de l'OSI : le transport des données.
    - Il est responsable de la réussite du transport des données.
    - Les protocoles courants de ce niveau d'abstraction sont TCP,
    - UPD.

RFC :
    - Publication des institutions technologiques décrivant
      les méthodes, les comportements et les innovations applicables au
      applicables au fonctionnement de l'Internet et des connexions Internet.

Socket TCP :
    - Mécanisme utilisé par les programmes pour accéder à un réseau.
    - Il permet de combiner des programmes sur des systèmes différents.

    Utilisation d'un socket TCP/IP :
    1. créer la socket :
    - int socket(int domain, int type, int protocol)
    - domaine : définit les autres sockets qui pourront s'y connecter.
      Le protocole permet de s'y connecter.
    - type : type de service, définit comment les informations seront échangées.
      Les informations seront échangées.
    - protocol : définit le protocole à utiliser par la socket,
      S'il est laissé à 0, le protocole standard de la famille du paramètre domain sera utilisé.
      du paramètre domain.

    Identifier la socket :
    - Attribuer un port à la socket, c'est ce qu'on appelle un bind.
      et se fait par un appel système.
    - int bind(int socket, const struct sockaddr *address, socklen_t address_len)
    - adress : structure qui stocke des informations sur l'adresse de la socket et le port auquel elle est destinée.
      La structure contient l'adresse du socket et le port à utiliser.
    - address_len : détermine la longueur de la structure adress.
    - Pour la mise en réseau, on utilise struct sockaddr_in :
        1. sin_family : doimio défini à la création.
        2. sin_port : le port à demander. Si vous ne définissez pas
            Si rien n'est défini, le système d'exploitation vous en attribue un.
        3. sin_addr/imgres : notre adresse IP. Si rien n'est défini
            vous est attribuée par le système d'exploitation.
    
    3. Attendre une connexion :
    - int listen(int socket, int backlog)
    - backlog : définit le nombre maximum de connexions qui peuvent être mises en attente avant de rejeter de nouvelles connexions.
      Le nombre de connexions en attente avant de rejeter de nouvelles connexions.
    - plus de choses à faire plus tard.

    4. Envoyer et recevoir des messages :
    - read -> socket (n'oubliez pas qu'il s'agit d'un fd)
    - écrire -> socket (rappelez-vous que c'est un fd)

    5. Fermer la socket :
    - close(socket) ;

HTTP :
    - client serveur : le serveur est notre serveur et le client peut être un navigateur (chrome, firefox, etc.).
      peut être un navigateur (chrome, firefox, etc.).
    - Le client envoie une requête HTTP au serveur et le serveur traite la requête et envoie une réponse.
      traite la requête et envoie une réponse.
    - Le client démarre la connexion.
    - La requête du client a une syntaxe (en-tête et corps).
    - Méthodes GET, POST, PUT :
        1. GET - le client envoie une demande avec une URL qui identifie la ressource à récupérer.
            L'URL identifie la ressource à récupérer.
        2. POST - le client envoie des données au serveur dans le corps du message.
            Il est utilisé pour créer de nouvelles ressources, mettre à jour des informations ou des données.
            mettre à jour des données.
        3. PUT - le client envoie une demande de mise à jour ou de remplacement des ressources du serveur qui sont spécifiées dans le corps du message.
            PUT - le client envoie une demande pour mettre à jour ou remplacer des ressources sur le serveur qui sont spécifiées dans le corps du message.
            Vous pouvez créer de nouvelles ressources si elles n'existaient pas auparavant sur le serveur, et le serveur utilisera l'URL de la ressource.
            Le serveur utilisera l'URL fournie.
     
     - Le serveur envoie une réponse avec un en-tête et une structure de corps donnés.
        Le corps de la réponse contient la structure de l'en-tête et du corps de la ressource. Le corps contiendra la ressource demandée si la réponse est réussie.
        est réussie.
      - Le serveur répondra avec la ressource attendue si la ressource existe,
        le serveur est actif et le client est autorisé à charger la ressource.
____________________________________________________________________________________________________________

SPANISH 🇪🇸

__________________________________________________________________________________________________
OSI:
    - Modelo conceptual. Es un estándar para que distintos
      sistemas se puedan comuncar entre sí y utiliza 
      7 niveles de abstracción.

    4to nivel de abstracción de OSI: Transporte de datos
    - Se encarga de que el transporte de datos sea exitoso.
    - Protocolos comunes de este nivel de abstracción son TCP,
    - UPD.

RFC:
    - Publicación de instituciones tecnológicas que describe
      métodos, comportamientos e innovaciones aplicables al
      funcionamiento de internet y de las conexiones de internet.

TCP socket:
    - Mecanismo que utilizan programas para tener acceso a una red.
    - Permite la cominación entre programas en diferentes sistemas.

    Uso de TCP/IP socket:
    1. Crear el socket:
    - int socket(int domain, int type, int protocol)
    - domain: establece qué otros sockets van a poder hacer
      conexión con él.
    - type: tipo de servicio, establece cómo se intercambiará la
      información.
    - protocol: establece el protocolo a utilizar por el socket,
      si se deja en 0, se utilizará el protocolo estándar de la familia
      del parámetro dominio.

    2. Identificar el socket:
    - Asignarle un puerto al socket, esto se llama bind
      y lo hace una llamada al sistema.
    - int bind(int socket, const struct sockaddr *address, socklen_t address_len)
    - adress: estructura que guarda información de la dirección del
      socket y del puerto que va a utilizar.
    - address_len: determina la longitud de la struct adress.
    - Para networking se utiliza struct sockaddr_in:
        1. sin_family: doimio establecido en la creación.
        2. sin_port: el puerto que vamos a solicictar. Si no se
            pone nada te asigna uno el OS.
        3. sin_addr/imgres: nuestra dirección IP. Si no se pone nada
            te asigna uno el OS.
    
    3. Esperar una conexión:
    - int listen(int socket, int backlog)
    - backlog: define el número máximo de conexiones que se pueden
      poner en cola antes de rechazar nuevas conexiones.
    - más cosas de accept que se harán luego.

    4. Mandar y recivir mensajes:
    - read -> socket (recordar que es un fd)
    - write -> socket (recordar que es un fd)

    5. Cerrar el socket:
    - close(socket);

HTTP:
    - cliente servidor: el servidor es nuestro servidor y el cliente
      puede ser un navegador(chrome, firefox, etc).
    - El cliente manda una petición HTTP al servidor y el servidor
      procesa la petición y envía una respuesta.
    - El cliente empieza la conexión.
    - La petición del cliente tiene una sintaxis (header and body).
    - Métodos GET, POST, PUT:
        1. GET - el cleinte envía una solicitud con una URL que identifica el
            recurso que se quiere conseguir.
        2. POST - el cleinte envía datos al servidor en el cuerpo del menasje.
            Se utiliza para crear nuevos recursos, actualizar información o
            actualizar datos.
        3. PUT -  el cliente envía una solicitud para actualizar o reemplazar
            recursos en el servidor, los cuales se especifican en el cuerpo.
            Puede crear nuevos recursos si no existían previamente en el
            servidor, y el servidor utilizará la URL proporcionada.
     
     - El servidor envía una respuesta con una estructura de header and body
        determinada. En el cuerpo estará el recurso solicitado si la respuesta
        es satisfactoria.
      - El servidor responderá con el recurso esperado cuando el recurso exista,
        el servidor esté activo y el cliente tenga permiso para cargar el recurso.
__________________________________________________________________________________________________
