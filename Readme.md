# PROGRAMACION DE MICROCONTROLADORES (PdM) - Jose Luis Rodriguez  CESE20Co23
                                             cel : + 57 316 657 87 46
                                             ing.joseluisrodriguez@gamil.com
                                             
Repositorio creado para el laboratorio de proyectos propuestos durante la cursada.

      WorkSpace IDE : STM32CubeIDE
Placa de desarrollo : NUCLEO-F429ZI


Para descargar respositorio.

git clone https://github.com/Joselios01/PdM_workspace.git

sobre la carpeta, luego abrirlo en STM32CubeIDE.


# DISTRIBUCION DE PINES EN LA BOARD NUCLEO-F429ZI

                    *******************************************************
                    *******************************************************
                    *******************************************************
       ETH_REF_CLK--** PA 1 **      #######################      ** PB 0 **-- LED1[GREEN]
          ETH_MDIO--** PA 2 **      # PLACA NUCLEO-F429ZI #      ** PB 7 **-- LED2[BLUE]
        ETH_CRS_DV--** PA 7 **      #######################      ** PB14 **-- LED3[RED]
         ETH_TX_EN--** PB11 **                                   **********
          ETH_TXD0--** PB12 **                                   ** PC13 **-- USER_BUTTON
          ETH_TXD1--** PB13 **     PINES USADOS POR LA PLACA     *********
           ETH_MDC--** PC 1 **                                   ** PD 8 **-- UART3_RX
          ETH_RXD0--** PC 4 **                                   ** PD 9 **-- UART3_TX
          ETH_RXD1--** PC 5 **                                   **********
                    *******************************************************
                    **********                                   **********
        DS1307_SCL--** PB 8 **                                   ** PA15 **-- SD_SPI1_CS
        DS1307_SDA--** PB 9 **                                   ** PB 3 **-- SD_SPI1_SCK
                    **********                                   ** PB 5 **-- SD_SPI1_MOSI
                    **********                                   ** PA 6 **-- SD_SPI1_MISO
                    **********    USADOS POR LA APLICACION       **********
                    **********                                   **********
                    **********                                   **********
                    *******************************************************
                    *******************************************************
                    *******************************************************
                    
                    
Proyectos relevantes:
funcionando

Trabajo 3-2 Uso de retardos no bloqueantes independientes Systick
Trabajo 4-2 Boton con retarno no bloqueante
Trabajo 5-2 manejo de UART
                    
                    
