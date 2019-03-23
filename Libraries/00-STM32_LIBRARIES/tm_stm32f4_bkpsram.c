/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_bkpsram.h"

void TM_BKPSRAM_Init(void) {
	/* Enable PWR clock */	
	__HAL_RCC_PWR_CLK_ENABLE();
	/* Enable backup SRAM Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;
	
	/* Enable the Backup SRAM low power Regulator */
	/* This will allow data to stay when using VBat mode */
	//PWR_BackupRegulatorCmd(ENABLE);
	
		/* Enable PWR peripheral clock */
//	RCC->CSR |= PWR_CSR_BRE;
//	MODIFY_REG(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS), PWR_LOWPOWERREGULATOR_ON);
	

	/* Allow access to BKP Domain */
	HAL_PWR_EnableBkUpAccess();
	*(__IO uint32_t *) CSR_BRE_BB = (uint32_t)ENABLE;

	
	/* Wait for backup regulator to be ready  */
	while (!(PWR->CSR & (PWR_FLAG_BRR)));
}
