#define BCM2835_PERI_BASE	0x3f000000
#define BCM2835_PERI_SIZE	0x01000000

#define BCM2835_ST_OFFSET	0x3000
#define BCM2835_CLOCK_OFFSET	0x101000
#define BCM2835_GPIO_OFFSET	0x200000
#define BCM2835_SPI0_OFFSET	0x204000
#define BCM2835_BSC0_OFFSET	0x205000
#define BCM2835_PWM_OFFSET	0x20C000
#define BCM2835_AUX_OFFSET	0x215000
#define BCM2835_SPI1_OFFSET	0x215080
#define BCM2835_SPI2_OFFSET	0x2150C0
#define BCM2835_BSC1_OFFSET	0x804000

#define BCM2835_ST_BASE		BCM2835_PERI_BASE+BCM2835_ST_OFFSET
#define BCM2835_CLOCK_BASE	BCM2835_PERI_BASE+BCM2835_CLOCK_OFFSET
#define BCM2835_GPIO_BASE	BCM2835_PERI_BASE+BCM2835_GPIO_OFFSET
#define BCM2835_SPI0_BASE	BCM2835_PERI_BASE+BCM2835_SPI0_OFFSET
#define BCM2835_BSC0_BASE	BCM2835_PERI_BASE+BCM2835_BSC0_OFFSET
#define BCM2835_PWM_BASE	BCM2835_PERI_BASE+BCM2835_PWM_OFFSET
#define BCM2835_AUX_BASE	BCM2835_PERI_BASE+BCM2835_AUX_OFFSET
#define BCM2835_SPI1_BASE	BCM2835_PERI_BASE+BCM2835_SPI1_OFFSET
#define BCM2835_SPI2_BASE	BCM2835_PERI_BASE+BCM2835_SPI2_OFFSET
#define BCM2835_BSC1_BASE	BCM2835_PERI_BASE+BCM2835_BSC1_OFFSET

#define BCM2835_GPFSEL0		0x0000 /*!< GPIO Function Select 0 */
#define BCM2835_GPFSEL1		0x0004 /*!< GPIO Function Select 1 */
#define BCM2835_GPFSEL2		0x0008 /*!< GPIO Function Select 2 */
#define BCM2835_GPFSEL3		0x000c /*!< GPIO Function Select 3 */
#define BCM2835_GPFSEL4		0x0010 /*!< GPIO Function Select 4 */
#define BCM2835_GPFSEL5		0x0014 /*!< GPIO Function Select 5 */
#define BCM2835_GPSET0		0x001c /*!< GPIO Pin Output Set 0 */
#define BCM2835_GPSET1		0x0020 /*!< GPIO Pin Output Set 1 */
#define BCM2835_GPCLR0		0x0028 /*!< GPIO Pin Output Clear 0 */
#define BCM2835_GPCLR1		0x002c /*!< GPIO Pin Output Clear 1 */
#define BCM2835_GPLEV0		0x0034 /*!< GPIO Pin Level 0 */
#define BCM2835_GPLEV1		0x0038 /*!< GPIO Pin Level 1 */
#define BCM2835_GPEDS0		0x0040 /*!< GPIO Pin Event Detect Status 0 */
#define BCM2835_GPEDS1		0x0044 /*!< GPIO Pin Event Detect Status 1 */
#define BCM2835_GPREN0		0x004c /*!< GPIO Pin Rising Edge Detect Enable 0 */
#define BCM2835_GPREN1		0x0050 /*!< GPIO Pin Rising Edge Detect Enable 1 */
#define BCM2835_GPFEN0		0x0058 /*!< GPIO Pin Falling Edge Detect Enable 0 */
#define BCM2835_GPFEN1		0x005c /*!< GPIO Pin Falling Edge Detect Enable 1 */
#define BCM2835_GPHEN0		0x0064 /*!< GPIO Pin High Detect Enable 0 */
#define BCM2835_GPHEN1		0x0068 /*!< GPIO Pin High Detect Enable 1 */
#define BCM2835_GPLEN0		0x0070 /*!< GPIO Pin Low Detect Enable 0 */
#define BCM2835_GPLEN1		0x0074 /*!< GPIO Pin Low Detect Enable 1 */
#define BCM2835_GPAREN0		0x007c /*!< GPIO Pin Async. Rising Edge Detect 0 */
#define BCM2835_GPAREN1		0x0080 /*!< GPIO Pin Async. Rising Edge Detect 1 */
#define BCM2835_GPAFEN0		0x0088 /*!< GPIO Pin Async. Falling Edge Detect 0 */
#define BCM2835_GPAFEN1		0x008c /*!< GPIO Pin Async. Falling Edge Detect 1 */
#define BCM2835_GPPUD		0x0094 /*!< GPIO Pin Pull-up/down Enable */
#define BCM2835_GPPUDCLK0	0x0098 /*!< GPIO Pin Pull-up/down Enable Clock 0 */
#define BCM2835_GPPUDCLK1	0x009c /*!< GPIO Pin Pull-up/down Enable Clock 1 */

#define BCM2835_GPIO_SIZE	0x00a0

#define BCM2835_PWM_CTL 	0x0
#define BCM2835_PWM_STA  	0x4
#define BCM2835_PWM_DMAC    	0x8
#define BCM2835_PWM_RNG1 	0x10
#define BCM2835_PWM_DAT1   	0x14
#define BCM2835_PWM_FIF1    	0x18
#define BCM2835_PWM_RNG2  	0x20
#define BCM2835_PWM_DAT2   	0x24

#define BCM2835_PWM_SIZE	0x28

#define BCM2835_PWMCLK_CNTL     40
#define BCM2835_PWMCLK_DIV      41
#define BCM2835_PWM_PASSWRD     (0x5A << 24)  /*!< Password to enable setting PWM clock */

#define BCM2835_ST_CS		0x0000 /*!< System Timer Control/Status */
#define BCM2835_ST_CLO		0x0004 /*!< System Timer Counter Lower 32 bits */
#define BCM2835_ST_CHI		0x0008 /*!< System Timer Counter Upper 32 bits */

#define MAGIC_NUMBER	'g'
#define IN		_IO(MAGIC_NUMBER,0)
#define OUT		_IO(MAGIC_NUMBER,1)
#define PP		_IO(MAGIC_NUMBER,2)
#define OC		_IO(MAGIC_NUMBER,3)
#define OUT_HIGH	_IO(MAGIC_NUMBER,4)
#define OUT_LOW		_IO(MAGIC_NUMBER,5)
#define IN_HIGH_DRAG	_IO(MAGIC_NUMBER,6)
#define IN_LOW_DRAG	_IO(MAGIC_NUMBER,7)
