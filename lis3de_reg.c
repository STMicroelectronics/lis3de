/**
  ******************************************************************************
  * @file    lis3de_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LIS3DE driver file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "lis3de_reg.h"

/**
  * @defgroup  LIS3DE
  * @brief     This file provides a set of functions needed to drive the
  *            lis3de enanced inertial module.
  * @{
  *
  */

/**
  * @defgroup  LIS3DE_Interfaces_Functions
  * @brief     This section provide a set of functions used to read and
  *            write a generic register of the device.
  *            MANDATORY: return 0 -> no Error.
  * @{
  *
  */

/**
  * @brief  Read generic device register
  *
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to read
  * @param  data  pointer to buffer that store the data read(ptr)
  * @param  len   number of consecutive register to read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak lis3de_read_reg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data,
                               uint16_t len)
{
  int32_t ret;

  ret = ctx->read_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @brief  Write generic device register
  *
  * @param  ctx   read / write interface definitions(ptr)
  * @param  reg   register to write
  * @param  data  pointer to data to write in register reg(ptr)
  * @param  len   number of consecutive register to write
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t __weak lis3de_write_reg(stmdev_ctx_t *ctx, uint8_t reg,
                                uint8_t *data,
                                uint16_t len)
{
  int32_t ret;

  ret = ctx->write_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LIS3DE_Sensitivity
  * @brief       These functions convert raw-data into engineering units.
  * @{
  *
  */

float_t lis3de_from_fs2_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 15.6f;
}

float_t lis3de_from_fs4_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 31.2f;
}

float_t lis3de_from_fs8_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 62.5f;
}

float_t lis3de_from_fs16_to_mg(int16_t lsb)
{
  return ((float_t)lsb) * 187.5f;
}

float_t lis3de_from_lsb_to_celsius(int16_t lsb)
{
  return (((float_t)lsb) * 1.0f) + 25.0f;
}

/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Data_generation
  * @brief     This section group all the functions concerning data generation.
  * @{
  *
  */

/**
  * @brief  Temperature status register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_temp_status_reg_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_STATUS_REG_AUX, buff, 1);

  return ret;
}
/**
  * @brief  Temperature data available.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tda in reg STATUS_REG_AUX
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_temp_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_status_reg_aux_t status_reg_aux;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_STATUS_REG_AUX,
                        (uint8_t *)&status_reg_aux, 1);
  *val = status_reg_aux._3da;

  return ret;
}
/**
  * @brief  Temperature data overrun.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tor in reg STATUS_REG_AUX
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_temp_data_ovr_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_status_reg_aux_t status_reg_aux;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_STATUS_REG_AUX,
                        (uint8_t *)&status_reg_aux, 1);
  *val = status_reg_aux._3or;

  return ret;
}
/**
  * @brief  Temperature output value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_temperature_raw_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_OUT_ADC1_H, buff, 1);

  return ret;
}

/**
  * @brief  ADC output value.[get]
  *         Sample frequency: the same as the ODR CTRL_REG1
  *         The resolution:
  *                    10bit if LPen bit in CTRL_REG1 (20h) is clear
  *                     8bit if LPen bit in CTRL_REG1 (20h) is set
  *         Data Format:
  *                     Outputs are Left Justified in 2’ complements
  *                     range 800mV
  *                     code zero means an analogue value of about 1.2V
  *                     Voltage values smaller than centre values are positive
  *                           (Example:  800mV = 7Fh / 127 dec)
  *                     Voltage values bigger than centre values are negative
  *                           (Example: 1600mV = 80h / -128 dec)
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_adc_raw_get(stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[6];
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_OUT_ADC1_L, buff, 6);
  val[0] = (int16_t)buff[1];
  val[0] = (val[0] * 256) + (int16_t)buff[0];
  val[1] = (int16_t)buff[3];
  val[1] = (val[1] * 256) + (int16_t)buff[2];
  val[2] = (int16_t)buff[5];
  val[2] = (val[2] * 256) + (int16_t)buff[4];

  return ret;
}

/**
  * @brief  Auxiliary ADC.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      configure the auxiliary ADC
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_aux_adc_set(stmdev_ctx_t *ctx, lis3de_temp_en_t val)
{
  lis3de_temp_cfg_reg_t temp_cfg_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TEMP_CFG_REG,
                        (uint8_t *)&temp_cfg_reg, 1);

  if (ret == 0)
  {
    if (val != LIS3DE_AUX_DISABLE)
    {
      /* Required in order to use auxiliary adc */
      ret = lis3de_block_data_update_set(ctx, PROPERTY_ENABLE);
    }
  }

  if (ret == 0)
  {
    temp_cfg_reg.temp_en = ((uint8_t) val & 0x02U) >> 1;
    temp_cfg_reg.adc_pd  = (uint8_t) val &  0x01U;
    ret = lis3de_write_reg(ctx, LIS3DE_TEMP_CFG_REG,
                           (uint8_t *)&temp_cfg_reg, 1);
  }

  return ret;
}

/**
  * @brief  Auxiliary ADC.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      configure the auxiliary ADC
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_aux_adc_get(stmdev_ctx_t *ctx, lis3de_temp_en_t *val)
{
  lis3de_temp_cfg_reg_t temp_cfg_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TEMP_CFG_REG,
                        (uint8_t *)&temp_cfg_reg, 1);

  if ((temp_cfg_reg.temp_en & temp_cfg_reg.adc_pd) ==
      PROPERTY_ENABLE)
  {
    *val = LIS3DE_AUX_ON_TEMPERATURE;
  }

  if ((temp_cfg_reg.temp_en  == PROPERTY_DISABLE) &&
      (temp_cfg_reg.adc_pd == PROPERTY_ENABLE))
  {
    *val = LIS3DE_AUX_ON_PADS;
  }

  else
  {
    *val = LIS3DE_AUX_DISABLE;
  }

  return ret;
}

/**
  * @brief  Operating mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of lpen in reg CTRL_REG1
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_operating_mode_set(stmdev_ctx_t *ctx,
                                  lis3de_op_md_t val)
{
  lis3de_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.lpen = (uint8_t) val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Operating mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of lpen in reg CTRL_REG1
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_operating_mode_get(stmdev_ctx_t *ctx,
                                  lis3de_op_md_t *val)
{
  lis3de_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    if (ctrl_reg1.lpen == PROPERTY_ENABLE)
    {
      *val = LIS3DE_LP;
    }

    else
    {
      *val = LIS3DE_NM;
    }
  }

  return ret;
}

/**
  * @brief  Output data rate selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of odr in reg CTRL_REG1
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_data_rate_set(stmdev_ctx_t *ctx, lis3de_odr_t val)
{
  lis3de_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.odr = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  Output data rate selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      get the values of odr in reg CTRL_REG1
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_data_rate_get(stmdev_ctx_t *ctx, lis3de_odr_t *val)
{
  lis3de_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.odr)
  {
    case LIS3DE_POWER_DOWN:
      *val = LIS3DE_POWER_DOWN;
      break;

    case LIS3DE_ODR_1Hz:
      *val = LIS3DE_ODR_1Hz;
      break;

    case LIS3DE_ODR_10Hz:
      *val = LIS3DE_ODR_10Hz;
      break;

    case LIS3DE_ODR_25Hz:
      *val = LIS3DE_ODR_25Hz;
      break;

    case LIS3DE_ODR_50Hz:
      *val = LIS3DE_ODR_50Hz;
      break;

    case LIS3DE_ODR_100Hz:
      *val = LIS3DE_ODR_100Hz;
      break;

    case LIS3DE_ODR_200Hz:
      *val = LIS3DE_ODR_200Hz;
      break;

    case LIS3DE_ODR_400Hz:
      *val = LIS3DE_ODR_400Hz;
      break;

    case LIS3DE_ODR_1kHz6:
      *val = LIS3DE_ODR_1kHz6;
      break;

    case LIS3DE_ODR_5kHz376_LP_1kHz344_NM:
      *val = LIS3DE_ODR_5kHz376_LP_1kHz344_NM;
      break;

    default:
      *val = LIS3DE_POWER_DOWN;
      break;
  }

  return ret;
}

/**
  * @brief   High pass data from internal filter sent to output register
  *          and FIFO.
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fds in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_on_outputs_set(stmdev_ctx_t *ctx,
                                        uint8_t val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.fds = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief   High pass data from internal filter sent to output register
  *          and FIFO.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fds in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_on_outputs_get(stmdev_ctx_t *ctx,
                                        uint8_t *val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = (uint8_t)ctrl_reg2.fds;

  return ret;
}

/**
  * @brief   High-pass filter cutoff frequency selection.[set]
  *
  * HPCF[2:1]\ft @1Hz    @10Hz  @25Hz  @50Hz @100Hz @200Hz @400Hz @1kHz6 ft@5kHz
  * AGGRESSIVE   0.02Hz  0.2Hz  0.5Hz  1Hz   2Hz    4Hz    8Hz    32Hz   100Hz
  * STRONG       0.008Hz 0.08Hz 0.2Hz  0.5Hz 1Hz    2Hz    4Hz    16Hz   50Hz
  * MEDIUM       0.004Hz 0.04Hz 0.1Hz  0.2Hz 0.5Hz  1Hz    2Hz    8Hz    25Hz
  * LIGHT        0.002Hz 0.02Hz 0.05Hz 0.1Hz 0.2Hz  0.5Hz  1Hz    4Hz    12Hz
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of hpcf in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_bandwidth_set(stmdev_ctx_t *ctx,
                                       lis3de_hpcf_t val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.hpcf = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief   High-pass filter cutoff frequency selection.[get]
  *
  * HPCF[2:1]\ft @1Hz    @10Hz  @25Hz  @50Hz @100Hz @200Hz @400Hz @1kHz6 ft@5kHz
  * AGGRESSIVE   0.02Hz  0.2Hz  0.5Hz  1Hz   2Hz    4Hz    8Hz    32Hz   100Hz
  * STRONG       0.008Hz 0.08Hz 0.2Hz  0.5Hz 1Hz    2Hz    4Hz    16Hz   50Hz
  * MEDIUM       0.004Hz 0.04Hz 0.1Hz  0.2Hz 0.5Hz  1Hz    2Hz    8Hz    25Hz
  * LIGHT        0.002Hz 0.02Hz 0.05Hz 0.1Hz 0.2Hz  0.5Hz  1Hz    4Hz    12Hz
  *
  * @param  ctx      read / write interface definitions
  * @param  val      get the values of hpcf in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_bandwidth_get(stmdev_ctx_t *ctx,
                                       lis3de_hpcf_t *val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  switch (ctrl_reg2.hpcf)
  {
    case LIS3DE_AGGRESSIVE:
      *val = LIS3DE_AGGRESSIVE;
      break;

    case LIS3DE_STRONG:
      *val = LIS3DE_STRONG;
      break;

    case LIS3DE_MEDIUM:
      *val = LIS3DE_MEDIUM;
      break;

    case LIS3DE_LIGHT:
      *val = LIS3DE_LIGHT;
      break;

    default:
      *val = LIS3DE_LIGHT;
      break;
  }

  return ret;
}

/**
  * @brief  High-pass filter mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of hpm in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_mode_set(stmdev_ctx_t *ctx, lis3de_hpm_t val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.hpm = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  High-pass filter mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      get the values of hpm in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_mode_get(stmdev_ctx_t *ctx,
                                  lis3de_hpm_t *val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  switch (ctrl_reg2.hpm)
  {
    case LIS3DE_NORMAL_WITH_RST:
      *val = LIS3DE_NORMAL_WITH_RST;
      break;

    case LIS3DE_REFERENCE_MODE:
      *val = LIS3DE_REFERENCE_MODE;
      break;

    case LIS3DE_NORMAL:
      *val = LIS3DE_NORMAL;
      break;

    case LIS3DE_AUTORST_ON_INT:
      *val = LIS3DE_AUTORST_ON_INT;
      break;

    default:
      *val = LIS3DE_NORMAL_WITH_RST;
      break;
  }

  return ret;
}

/**
  * @brief  Full-scale configuration.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fs in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_full_scale_set(stmdev_ctx_t *ctx, lis3de_fs_t val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  if (ret == 0)
  {
    ctrl_reg4.fs = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  }

  return ret;
}

/**
  * @brief  Full-scale configuration.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      get the values of fs in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_full_scale_get(stmdev_ctx_t *ctx, lis3de_fs_t *val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  switch (ctrl_reg4.fs)
  {
    case LIS3DE_2g:
      *val = LIS3DE_2g;
      break;

    case LIS3DE_4g:
      *val = LIS3DE_4g;
      break;

    case LIS3DE_8g:
      *val = LIS3DE_8g;
      break;

    case LIS3DE_16g:
      *val = LIS3DE_16g;
      break;

    default:
      *val = LIS3DE_2g;
      break;
  }

  return ret;
}

/**
  * @brief  Block Data Update.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of bdu in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_block_data_update_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  if (ret == 0)
  {
    ctrl_reg4.bdu = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  }

  return ret;
}

/**
  * @brief  Block Data Update.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of bdu in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_block_data_update_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  *val = (uint8_t)ctrl_reg4.bdu;

  return ret;
}

/**
  * @brief  Reference value for interrupt generation.[set]
  *         LSB = ~16@2g / ~31@4g / ~63@8g / ~127@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that contains data to write
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_filter_reference_set(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis3de_write_reg(ctx, LIS3DE_REFERENCE, buff, 1);

  return ret;
}

/**
  * @brief  Reference value for interrupt generation.[get]
  *         LSB = ~16@2g / ~31@4g / ~63@8g / ~127@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_filter_reference_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_REFERENCE, buff, 1);

  return ret;
}
/**
  * @brief  Acceleration set of data available.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of zyxda in reg STATUS_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_xl_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_status_reg_t status_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_STATUS_REG, (uint8_t *)&status_reg, 1);
  *val = status_reg.zyxda;

  return ret;
}
/**
  * @brief  Acceleration set of data overrun.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of zyxor in reg STATUS_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_xl_data_ovr_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_status_reg_t status_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_STATUS_REG, (uint8_t *)&status_reg, 1);
  *val = status_reg.zyxor;

  return ret;
}
/**
  * @brief  Acceleration output value.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_acceleration_raw_get(stmdev_ctx_t *ctx, int16_t *buff)
{
  int32_t ret;

  int8_t dummy;
  ret = lis3de_read_reg(ctx, LIS3DE_OUT_X, (uint8_t *)&dummy, 1);
  buff[0] = dummy;

  if (ret == 0)
  {
    ret = lis3de_read_reg(ctx, LIS3DE_OUT_Y, (uint8_t *)&dummy, 1);
    buff[1] = dummy;
  }

  if (ret == 0)
  {
    ret = lis3de_read_reg(ctx, LIS3DE_OUT_Z, (uint8_t *)&dummy, 1);
    buff[2] = dummy;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Common
  * @brief     This section group common useful functions
  * @{
  *
  */

/**
  * @brief  DeviceWhoamI .[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  buff     buffer that stores data read
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_WHO_AM_I, buff, 1);

  return ret;
}
/**
  * @brief  Self Test.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of st in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_self_test_set(stmdev_ctx_t *ctx, lis3de_st_t val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  if (ret == 0)
  {
    ctrl_reg4.st = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  }

  return ret;
}

/**
  * @brief  Self Test.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of st in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_self_test_get(stmdev_ctx_t *ctx, lis3de_st_t *val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  switch (ctrl_reg4.st)
  {
    case LIS3DE_ST_DISABLE:
      *val = LIS3DE_ST_DISABLE;
      break;

    case LIS3DE_ST_POSITIVE:
      *val = LIS3DE_ST_POSITIVE;
      break;

    case LIS3DE_ST_NEGATIVE:
      *val = LIS3DE_ST_NEGATIVE;
      break;

    default:
      *val = LIS3DE_ST_DISABLE;
      break;
  }

  return ret;
}

/**
  * @brief  Reboot memory content. Reload the calibration parameters.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of boot in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_boot_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  if (ret == 0)
  {
    ctrl_reg5.boot = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  }

  return ret;
}

/**
  * @brief  Reboot memory content. Reload the calibration parameters.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of boot in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_boot_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  *val = (uint8_t)ctrl_reg5.boot;

  return ret;
}

/**
  * @brief  Info about device status.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      register STATUS_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_status_get(stmdev_ctx_t *ctx, lis3de_status_reg_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_STATUS_REG, (uint8_t *) val, 1);

  return ret;
}
/**
  * @}
  *
  */

/**
  * @defgroup   LIS3DE_Interrupts_generator_1
  * @brief      This section group all the functions that manage the first
  *             interrupts generator
  * @{
  *
  */

/**
  * @brief  Interrupt generator 1 configuration register.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      register INT1_CFG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_conf_set(stmdev_ctx_t *ctx,
                                 lis3de_ig1_cfg_t *val)
{
  int32_t ret;

  ret = lis3de_write_reg(ctx, LIS3DE_IG1_CFG, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Interrupt generator 1 configuration register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      register INT1_CFG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_conf_get(stmdev_ctx_t *ctx,
                                 lis3de_ig1_cfg_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG1_CFG, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Interrupt generator 1 source register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Registers INT1_SRC
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_source_get(stmdev_ctx_t *ctx,
                                   lis3de_ig1_source_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG1_SOURCE, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief  User-defined threshold value for xl interrupt event on
  *         generator 1.[set]
  *         LSb = 16mg@2g / 32mg@4g / 62mg@8g / 186mg@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ths in reg INT1_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_threshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ig1_ths_t int1_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG1_THS, (uint8_t *)&int1_ths, 1);

  if (ret == 0)
  {
    int1_ths.ths = val;
    ret = lis3de_write_reg(ctx, LIS3DE_IG1_THS, (uint8_t *)&int1_ths, 1);
  }

  return ret;
}

/**
  * @brief  User-defined threshold value for xl interrupt event on
  *         generator 1.[get]
  *         LSb = 16mg@2g / 32mg@4g / 62mg@8g / 186mg@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ths in reg INT1_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_threshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ig1_ths_t int1_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG1_THS, (uint8_t *)&int1_ths, 1);
  *val = (uint8_t)int1_ths.ths;

  return ret;
}

/**
  * @brief  The minimum duration (LSb = 1/ODR) of the Interrupt 1 event to be
  *         recognized.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d in reg INT1_DURATION
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_duration_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ig1_duration_t int1_duration;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG1_DURATION,
                        (uint8_t *)&int1_duration, 1);

  if (ret == 0)
  {
    int1_duration.d = val;
    ret = lis3de_write_reg(ctx, LIS3DE_IG1_DURATION,
                           (uint8_t *)&int1_duration, 1);
  }

  return ret;
}

/**
  * @brief  The minimum duration (LSb = 1/ODR) of the Interrupt 1 event to be
  *         recognized.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d in reg INT1_DURATION
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_gen_duration_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ig1_duration_t int1_duration;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG1_DURATION,
                        (uint8_t *)&int1_duration, 1);
  *val = (uint8_t)int1_duration.d;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup   LIS3DE_Interrupts_generator_2
  * @brief      This section group all the functions that manage the second
  *             interrupts generator
  * @{
  *
  */

/**
  * @brief  Interrupt generator 2 configuration register.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers IG2_CFG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_conf_set(stmdev_ctx_t *ctx,
                                 lis3de_ig2_cfg_t *val)
{
  int32_t ret;

  ret = lis3de_write_reg(ctx, LIS3DE_IG2_CFG, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Interrupt generator 2 configuration register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers IG2_CFG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_conf_get(stmdev_ctx_t *ctx,
                                 lis3de_ig2_cfg_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG2_CFG, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief  Interrupt generator 2 source register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers INT2_SRC
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_source_get(stmdev_ctx_t *ctx,
                                   lis3de_ig2_source_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG2_SOURCE, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief   User-defined threshold value for xl interrupt event on
  *          generator 2.[set]
  *          LSb = 16mg@2g / 32mg@4g / 62mg@8g / 186mg@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ths in reg INT2_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_threshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ig2_ths_t int2_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG2_THS, (uint8_t *)&int2_ths, 1);

  if (ret == 0)
  {
    int2_ths.ths = val;
    ret = lis3de_write_reg(ctx, LIS3DE_IG2_THS, (uint8_t *)&int2_ths, 1);
  }

  return ret;
}

/**
  * @brief  User-defined threshold value for xl interrupt event on
  *         generator 2.[get]
  *         LSb = 16mg@2g / 32mg@4g / 62mg@8g / 186mg@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ths in reg INT2_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_threshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ig2_ths_t int2_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG2_THS, (uint8_t *)&int2_ths, 1);
  *val = (uint8_t)int2_ths.ths;

  return ret;
}

/**
  * @brief  The minimum duration (LSb = 1/ODR) of the Interrupt 1 event to be
  *         recognized .[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d in reg INT2_DURATION
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_duration_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ig2_duration_t int2_duration;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG2_DURATION,
                        (uint8_t *)&int2_duration, 1);

  if (ret == 0)
  {
    int2_duration.d = val;
    ret = lis3de_write_reg(ctx, LIS3DE_IG2_DURATION,
                           (uint8_t *)&int2_duration, 1);
  }

  return ret;
}

/**
  * @brief  The minimum duration (LSb = 1/ODR) of the Interrupt 1 event to be
  *         recognized.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d in reg INT2_DURATION
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_gen_duration_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ig2_duration_t int2_duration;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_IG2_DURATION,
                        (uint8_t *)&int2_duration, 1);
  *val = (uint8_t)int2_duration.d;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Interrupt_pins
  * @brief     This section group all the functions that manage interrupt pins
  * @{
  *
  */

/**
  * @brief  High-pass filter on interrupts/tap generator.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of hp in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_int_conf_set(stmdev_ctx_t *ctx,
                                      lis3de_hp_t val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.hp = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  High-pass filter on interrupts/tap generator.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of hp in reg CTRL_REG2
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_high_pass_int_conf_get(stmdev_ctx_t *ctx,
                                      lis3de_hp_t *val)
{
  lis3de_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  switch (ctrl_reg2.hp)
  {
    case LIS3DE_DISC_FROM_INT_GENERATOR:
      *val = LIS3DE_DISC_FROM_INT_GENERATOR;
      break;

    case LIS3DE_ON_INT1_GEN:
      *val = LIS3DE_ON_INT1_GEN;
      break;

    case LIS3DE_ON_INT2_GEN:
      *val = LIS3DE_ON_INT2_GEN;
      break;

    case LIS3DE_ON_TAP_GEN:
      *val = LIS3DE_ON_TAP_GEN;
      break;

    case LIS3DE_ON_INT1_INT2_GEN:
      *val = LIS3DE_ON_INT1_INT2_GEN;
      break;

    case LIS3DE_ON_INT1_TAP_GEN:
      *val = LIS3DE_ON_INT1_TAP_GEN;
      break;

    case LIS3DE_ON_INT2_TAP_GEN:
      *val = LIS3DE_ON_INT2_TAP_GEN;
      break;

    case LIS3DE_ON_INT1_INT2_TAP_GEN:
      *val = LIS3DE_ON_INT1_INT2_TAP_GEN;
      break;

    default:
      *val = LIS3DE_DISC_FROM_INT_GENERATOR;
      break;
  }

  return ret;
}

/**
  * @brief  Int1 pin routing configuration register.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CTRL_REG3
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_pin_int1_config_set(stmdev_ctx_t *ctx,
                                   lis3de_ctrl_reg3_t *val)
{
  int32_t ret;

  ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG3, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Int1 pin routing configuration register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CTRL_REG3
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_pin_int1_config_get(stmdev_ctx_t *ctx,
                                   lis3de_ctrl_reg3_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG3, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief  int2_pin_detect_4d: [set]  4D enable: 4D detection is enabled
  *                                    on INT2 pin when 6D bit on
  *                                    IG2_CFG (34h) is set to 1.
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d4d_ig2 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_pin_detect_4d_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  if (ret == 0)
  {
    ctrl_reg5.d4d_ig2 = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  }

  return ret;
}

/**
  * @brief  4D enable: 4D detection is enabled on INT2 pin when 6D bit on
  *         IG2_CFG (34h) is set to 1.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d4d_ig2 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_pin_detect_4d_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  *val = (uint8_t)ctrl_reg5.d4d_ig2;

  return ret;
}

/**
  * @brief   Latch interrupt request on INT2_SRC (35h) register, with
  *          INT2_SRC (35h) register cleared by reading INT2_SRC(35h)
  *          itself.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of lir_ig2 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_pin_notification_mode_set(stmdev_ctx_t *ctx,
                                              lis3de_lir_int2_t val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  if (ret == 0)
  {
    ctrl_reg5.lir_ig2 = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  }

  return ret;
}

/**
  * @brief   Latch interrupt request on INT2_SRC (35h) register, with
  *          INT2_SRC (35h) register cleared by reading INT2_SRC(35h)
  *          itself.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of lir_ig2 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int2_pin_notification_mode_get(stmdev_ctx_t *ctx,
                                              lis3de_lir_int2_t *val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  switch (ctrl_reg5.lir_ig2)
  {
    case LIS3DE_INT2_PULSED:
      *val = LIS3DE_INT2_PULSED;
      break;

    case LIS3DE_INT2_LATCHED:
      *val = LIS3DE_INT2_LATCHED;
      break;

    default:
      *val = LIS3DE_INT2_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  4D enable: 4D detection is enabled on INT1 pin when 6D bit
  *                    on INT1_CFG(30h) is set to 1.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d4d_ig1 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_pin_detect_4d_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  if (ret == 0)
  {
    ctrl_reg5.d4d_ig1 = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  }

  return ret;
}

/**
  * @brief  4D enable: 4D detection is enabled on INT1 pin when 6D bit on
  *         INT1_CFG(30h) is set to 1.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of d4d_ig1 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_pin_detect_4d_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  *val = (uint8_t)ctrl_reg5.d4d_ig1;

  return ret;
}

/**
  * @brief   Latch interrupt request on INT1_SRC (31h), with INT1_SRC(31h)
  *          register cleared by reading INT1_SRC (31h) itself.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of lir_ig1 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_pin_notification_mode_set(stmdev_ctx_t *ctx,
                                              lis3de_lir_int1_t val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  if (ret == 0)
  {
    ctrl_reg5.lir_ig1 = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  }

  return ret;
}

/**
  * @brief   Latch interrupt request on INT1_SRC (31h), with INT1_SRC(31h)
  *          register cleared by reading INT1_SRC (31h) itself.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of lir_ig1 in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_int1_pin_notification_mode_get(stmdev_ctx_t *ctx,
                                              lis3de_lir_int1_t *val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  switch (ctrl_reg5.lir_ig1)
  {
    case LIS3DE_INT1_PULSED:
      *val = LIS3DE_INT1_PULSED;
      break;

    case LIS3DE_INT1_LATCHED:
      *val = LIS3DE_INT1_LATCHED;
      break;

    default:
      *val = LIS3DE_INT1_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  Int2 pin routing configuration register.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CTRL_REG6
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_pin_int2_config_set(stmdev_ctx_t *ctx,
                                   lis3de_ctrl_reg6_t *val)
{
  int32_t ret;

  ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG6, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Int2 pin routing configuration register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CTRL_REG6
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_pin_int2_config_get(stmdev_ctx_t *ctx,
                                   lis3de_ctrl_reg6_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG6, (uint8_t *) val, 1);

  return ret;
}
/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Fifo
  * @brief     This section group all the functions concerning the fifo usage
  * @{
  *
  */

/**
  * @brief  FIFO enable.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fifo_en in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);

  if (ret == 0)
  {
    ctrl_reg5.fifo_en = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  }

  return ret;
}

/**
  * @brief  FIFO enable.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fifo_en in reg CTRL_REG5
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG5, (uint8_t *)&ctrl_reg5, 1);
  *val = (uint8_t)ctrl_reg5.fifo_en;

  return ret;
}

/**
  * @brief  FIFO watermark level selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fth in reg FIFO_CTRL_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_watermark_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_fifo_ctrl_reg_t fifo_ctrl_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                        (uint8_t *)&fifo_ctrl_reg, 1);

  if (ret == 0)
  {
    fifo_ctrl_reg.fth = val;
    ret = lis3de_write_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                           (uint8_t *)&fifo_ctrl_reg, 1);
  }

  return ret;
}

/**
  * @brief  FIFO watermark level selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fth in reg FIFO_CTRL_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_watermark_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_fifo_ctrl_reg_t fifo_ctrl_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                        (uint8_t *)&fifo_ctrl_reg, 1);
  *val = (uint8_t)fifo_ctrl_reg.fth;

  return ret;
}

/**
  * @brief  Trigger FIFO selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tr in reg FIFO_CTRL_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_trigger_event_set(stmdev_ctx_t *ctx,
                                      lis3de_tr_t val)
{
  lis3de_fifo_ctrl_reg_t fifo_ctrl_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                        (uint8_t *)&fifo_ctrl_reg, 1);

  if (ret == 0)
  {
    fifo_ctrl_reg.tr = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                           (uint8_t *)&fifo_ctrl_reg, 1);
  }

  return ret;
}

/**
  * @brief  Trigger FIFO selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of tr in reg FIFO_CTRL_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_trigger_event_get(stmdev_ctx_t *ctx,
                                      lis3de_tr_t *val)
{
  lis3de_fifo_ctrl_reg_t fifo_ctrl_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                        (uint8_t *)&fifo_ctrl_reg, 1);

  switch (fifo_ctrl_reg.tr)
  {
    case LIS3DE_INT1_GEN:
      *val = LIS3DE_INT1_GEN;
      break;

    case LIS3DE_INT2_GEN:
      *val = LIS3DE_INT2_GEN;
      break;

    default:
      *val = LIS3DE_INT1_GEN;
      break;
  }

  return ret;
}

/**
  * @brief  FIFO mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fm in reg FIFO_CTRL_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_mode_set(stmdev_ctx_t *ctx, lis3de_fm_t val)
{
  lis3de_fifo_ctrl_reg_t fifo_ctrl_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                        (uint8_t *)&fifo_ctrl_reg, 1);

  if (ret == 0)
  {
    fifo_ctrl_reg.fm = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                           (uint8_t *)&fifo_ctrl_reg, 1);
  }

  return ret;
}

/**
  * @brief  FIFO mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of fm in reg FIFO_CTRL_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_mode_get(stmdev_ctx_t *ctx, lis3de_fm_t *val)
{
  lis3de_fifo_ctrl_reg_t fifo_ctrl_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_CTRL_REG,
                        (uint8_t *)&fifo_ctrl_reg, 1);

  switch (fifo_ctrl_reg.fm)
  {
    case LIS3DE_BYPASS_MODE:
      *val = LIS3DE_BYPASS_MODE;
      break;

    case LIS3DE_FIFO_MODE:
      *val = LIS3DE_FIFO_MODE;
      break;

    case LIS3DE_DYNAMIC_STREAM_MODE:
      *val = LIS3DE_DYNAMIC_STREAM_MODE;
      break;

    case LIS3DE_STREAM_TO_FIFO_MODE:
      *val = LIS3DE_STREAM_TO_FIFO_MODE;
      break;

    default:
      *val = LIS3DE_BYPASS_MODE;
      break;
  }

  return ret;
}

/**
  * @brief  FIFO status register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers FIFO_SRC_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_status_get(stmdev_ctx_t *ctx,
                               lis3de_fifo_src_reg_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_SRC_REG, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief  FIFO stored data level.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of fss in reg FIFO_SRC_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_data_level_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_fifo_src_reg_t fifo_src_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_SRC_REG,
                        (uint8_t *)&fifo_src_reg, 1);
  *val = (uint8_t)fifo_src_reg.fss;

  return ret;
}
/**
  * @brief  Empty FIFO status flag.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of empty in reg FIFO_SRC_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_empty_flag_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_fifo_src_reg_t fifo_src_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_SRC_REG,
                        (uint8_t *)&fifo_src_reg, 1);
  *val = (uint8_t)fifo_src_reg.empty;

  return ret;
}
/**
  * @brief  FIFO overrun status flag.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ovrn_fifo in reg FIFO_SRC_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_ovr_flag_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_fifo_src_reg_t fifo_src_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_SRC_REG,
                        (uint8_t *)&fifo_src_reg, 1);
  *val = (uint8_t)fifo_src_reg.ovrn_fifo;

  return ret;
}
/**
  * @brief  FIFO watermark status.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of wtm in reg FIFO_SRC_REG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_fifo_fth_flag_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_fifo_src_reg_t fifo_src_reg;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_FIFO_SRC_REG,
                        (uint8_t *)&fifo_src_reg, 1);
  *val = (uint8_t)fifo_src_reg.wtm;

  return ret;
}
/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Tap_generator
  * @brief     This section group all the functions that manage the tap and
  *            double tap event generation
  * @{
  *
  */

/**
  * @brief  Tap/Double Tap generator configuration register.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CLICK_CFG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_conf_set(stmdev_ctx_t *ctx,
                            lis3de_click_cfg_t *val)
{
  int32_t ret;

  ret = lis3de_write_reg(ctx, LIS3DE_CLICK_CFG, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  Tap/Double Tap generator configuration register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CLICK_CFG
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_conf_get(stmdev_ctx_t *ctx,
                            lis3de_click_cfg_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CLICK_CFG, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief  Tap/Double Tap generator source register.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      registers CLICK_SRC
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_source_get(stmdev_ctx_t *ctx,
                              lis3de_click_src_t *val)
{
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CLICK_SRC, (uint8_t *) val, 1);

  return ret;
}
/**
  * @brief  User-defined threshold value for Tap/Double Tap event.[set]
  *         1 LSB = full scale/128
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ths in reg CLICK_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_threshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_click_ths_t click_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CLICK_THS, (uint8_t *)&click_ths, 1);

  if (ret == 0)
  {
    click_ths.ths = val;
    ret = lis3de_write_reg(ctx, LIS3DE_CLICK_THS, (uint8_t *)&click_ths, 1);
  }

  return ret;
}

/**
  * @brief  User-defined threshold value for Tap/Double Tap event.[get]
  *         1 LSB = full scale/128
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of ths in reg CLICK_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_threshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_click_ths_t click_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CLICK_THS, (uint8_t *)&click_ths, 1);
  *val = (uint8_t)click_ths.ths;

  return ret;
}

/**
  * @brief   If the LIR_Click bit is not set, the interrupt is kept high
  *          for the duration of the latency window.
  *          If the LIR_Click bit is set, the interrupt is kept high until the
  *          CLICK_SRC(39h) register is read.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of lir in reg CLICK_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_notification_mode_set(stmdev_ctx_t *ctx,
                                         lis3de_lir_t val)
{
  lis3de_click_ths_t click_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CLICK_THS, (uint8_t *)&click_ths, 1);

  if (ret == 0)
  {
    click_ths.lir = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CLICK_THS, (uint8_t *)&click_ths, 1);
  }

  return ret;
}

/**
  * @brief   If the LIR_Click bit is not set, the interrupt is kept high
  *          for the duration of the latency window.
  *          If the LIR_Click bit is set, the interrupt is kept high until the
  *          CLICK_SRC(39h) register is read.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of lir in reg CLICK_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_tap_notification_mode_get(stmdev_ctx_t *ctx,
                                         lis3de_lir_t *val)
{
  lis3de_click_ths_t click_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CLICK_THS, (uint8_t *)&click_ths, 1);

  switch (click_ths.lir)
  {
    case LIS3DE_TAP_PULSED:
      *val = LIS3DE_TAP_PULSED;
      break;

    case LIS3DE_TAP_LATCHED:
      *val = LIS3DE_TAP_LATCHED;
      break;

    default:
      *val = LIS3DE_TAP_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  The maximum time (1 LSB = 1/ODR) interval that can elapse
  *         between the start of the click-detection procedure and when the
  *         acceleration falls back below the threshold.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tli in reg TIME_LIMIT
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_shock_dur_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_time_limit_t time_limit;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TIME_LIMIT, (uint8_t *)&time_limit, 1);

  if (ret == 0)
  {
    time_limit.tli = val;
    ret = lis3de_write_reg(ctx, LIS3DE_TIME_LIMIT, (uint8_t *)&time_limit, 1);
  }

  return ret;
}

/**
  * @brief  The maximum time (1 LSB = 1/ODR) interval that can elapse between
  *         the start of the click-detection procedure and when the
  *         acceleration falls back below the threshold.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tli in reg TIME_LIMIT
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_shock_dur_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_time_limit_t time_limit;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TIME_LIMIT, (uint8_t *)&time_limit, 1);
  *val = (uint8_t)time_limit.tli;

  return ret;
}

/**
  * @brief  The time (1 LSB = 1/ODR) interval that starts after the first
  *         click detection where the click-detection procedure is
  *         disabled, in cases where the device is configured for
  *         double-click detection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tla in reg TIME_LATENCY
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_quiet_dur_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_time_latency_t time_latency;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TIME_LATENCY,
                        (uint8_t *)&time_latency, 1);

  if (ret == 0)
  {
    time_latency.tla = val;
    ret = lis3de_write_reg(ctx, LIS3DE_TIME_LATENCY,
                           (uint8_t *)&time_latency, 1);
  }

  return ret;
}

/**
  * @brief  The time (1 LSB = 1/ODR) interval that starts after the first
  *         click detection where the click-detection procedure is
  *         disabled, in cases where the device is configured for
  *         double-click detection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tla in reg TIME_LATENCY
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_quiet_dur_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_time_latency_t time_latency;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TIME_LATENCY,
                        (uint8_t *)&time_latency, 1);
  *val = (uint8_t)time_latency.tla;

  return ret;
}

/**
  * @brief  The maximum interval of time (1 LSB = 1/ODR) that can elapse
  *         after the end of the latency interval in which the click-detection
  *         procedure can start, in cases where the device is configured
  *         for double-click detection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tw in reg TIME_WINDOW
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_double_tap_timeout_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_time_window_t time_window;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TIME_WINDOW,
                        (uint8_t *)&time_window, 1);

  if (ret == 0)
  {
    time_window.tw = val;
    ret = lis3de_write_reg(ctx, LIS3DE_TIME_WINDOW,
                           (uint8_t *)&time_window, 1);
  }

  return ret;
}

/**
  * @brief  The maximum interval of time (1 LSB = 1/ODR) that can elapse
  *         after the end of the latency interval in which the
  *         click-detection procedure can start, in cases where the device
  *         is configured for double-click detection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of tw in reg TIME_WINDOW
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_double_tap_timeout_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_time_window_t time_window;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_TIME_WINDOW,
                        (uint8_t *)&time_window, 1);
  *val = (uint8_t)time_window.tw;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Activity_inactivity
  * @brief     This section group all the functions concerning activity
  *            inactivity functionality
  * @{
  *
  */

/**
  * @brief    Sleep-to-wake, return-to-sleep activation threshold in
  *           low-power mode.[set]
  *           1 LSb = 16mg@2g / 32mg@4g / 62mg@8g / 186mg@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of acth in reg ACT_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_act_threshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_act_ths_t act_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_ACT_THS, (uint8_t *)&act_ths, 1);

  if (ret == 0)
  {
    act_ths.acth = val;
    ret = lis3de_write_reg(ctx, LIS3DE_ACT_THS, (uint8_t *)&act_ths, 1);
  }

  return ret;
}

/**
  * @brief  Sleep-to-wake, return-to-sleep activation threshold in low-power
  *         mode.[get]
  *         1 LSb = 16mg@2g / 32mg@4g / 62mg@8g / 186mg@16g
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of acth in reg ACT_THS
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_act_threshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_act_ths_t act_ths;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_ACT_THS, (uint8_t *)&act_ths, 1);
  *val = (uint8_t)act_ths.acth;

  return ret;
}

/**
  * @brief  Sleep-to-wake, return-to-sleep.[set]
  *         duration = (8*1[LSb]+1)/ODR
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of actd in reg ACT_DUR
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_act_timeout_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis3de_act_dur_t act_dur;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_ACT_DUR, (uint8_t *)&act_dur, 1);

  if (ret == 0)
  {
    act_dur.actd = val;
    ret = lis3de_write_reg(ctx, LIS3DE_ACT_DUR, (uint8_t *)&act_dur, 1);
  }

  return ret;
}

/**
  * @brief  Sleep-to-wake, return-to-sleep.[get]
  *         duration = (8*1[LSb]+1)/ODR
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of actd in reg ACT_DUR
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_act_timeout_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis3de_act_dur_t act_dur;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_ACT_DUR, (uint8_t *)&act_dur, 1);
  *val = (uint8_t)act_dur.actd;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup  LIS3DE_Serial_interface
  * @brief     This section group all the functions concerning serial
  *            interface management
  * @{
  *
  */

/**
  * @brief  SPI Serial Interface Mode selection.[set]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      change the values of sim in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_spi_mode_set(stmdev_ctx_t *ctx, lis3de_sim_t val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  if (ret == 0)
  {
    ctrl_reg4.sim = (uint8_t)val;
    ret = lis3de_write_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);
  }

  return ret;
}

/**
  * @brief  SPI Serial Interface Mode selection.[get]
  *
  * @param  ctx      read / write interface definitions
  * @param  val      Get the values of sim in reg CTRL_REG4
  * @retval          interface status (MANDATORY: return 0 -> no Error)
  *
  */
int32_t lis3de_spi_mode_get(stmdev_ctx_t *ctx, lis3de_sim_t *val)
{
  lis3de_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis3de_read_reg(ctx, LIS3DE_CTRL_REG4, (uint8_t *)&ctrl_reg4, 1);

  switch (ctrl_reg4.sim)
  {
    case LIS3DE_SPI_4_WIRE:
      *val = LIS3DE_SPI_4_WIRE;
      break;

    case LIS3DE_SPI_3_WIRE:
      *val = LIS3DE_SPI_3_WIRE;
      break;

    default:
      *val = LIS3DE_SPI_4_WIRE;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
