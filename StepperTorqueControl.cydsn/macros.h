/* ========================================
 * Defines for different parts of the project
 * ========================================
*/

//AS5048 macros
#define SPI_CMD_READ 0x4000 /*!< flag indicating read attempt when using SPI interface */
#define SPI_REG_AGC 0x3ffd /*!< agc register when using SPI */
#define SPI_REG_MAG 0x3ffe /*!< magnitude register when using SPI */
#define SPI_REG_DATA 0x3fff /*!< data register when using SPI */
#define SPI_REG_CLRERR 0x1 /*!< clear error register when using SPI */

//microstep macros
#define FULL_STEP (0u)
#define HALF_STEP (1u)
#define QUARTER_STEP (2u)
#define EIGHTH_STEP (3u)
#define SIXTEENTH_STEP (7u)

/* [] END OF FILE */
