REVOKE ALL PRIVILEGES ON * . * FROM 'warhead'@'localhost';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'warhead'@'localhost';

REVOKE GRANT OPTION ON `world` . * FROM 'warhead'@'localhost';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'warhead'@'localhost';

REVOKE GRANT OPTION ON `characters` . * FROM 'warhead'@'localhost';

REVOKE ALL PRIVILEGES ON `auth` . * FROM 'warhead'@'localhost';

REVOKE GRANT OPTION ON `auth` . * FROM 'warhead'@'localhost';

DROP USER 'warhead'@'localhost';

DROP DATABASE IF EXISTS `world`;

DROP DATABASE IF EXISTS `characters`;

DROP DATABASE IF EXISTS `auth`;
