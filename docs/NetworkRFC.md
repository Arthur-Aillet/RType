# RFC Describing Network operations for the RType Game
--
## Basis:

RType is based on the C++evy engine, and thus uses its network protocol
see the relevant RFC for the base

## Actions

Rtype defines and uses the following actions

|   Action   |   Value   |        Data        |  Data Size  |
|------------|-----------|--------------------|-------------|
|    Shoot   |    0x01   |         {}         |   0 bytes   |
|     Fly    |    0x02   | Destination Vector | 3 * 4 bytes |

## Archetypes

Rtype defines and uses the following archetypes

|  Archetype  |   Value   |
|-------------|-----------|
| PlayerShip  |   0x00    |
|     Ship    |   0x01    |
|    Enemy    |   0x02    |
|    Bullet   |   0x03    |
