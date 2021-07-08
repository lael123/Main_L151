#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <spi_flash.h>
#include <drv_spi.h>
#include "rthw.h"
#include "pin_config.h"
#include "FlashWork.h"
#include "status.h"
#include "led.h"
#include <flashdb.h>

#define DBG_TAG "flash"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define FDB_LOG_TAG "[sample][kvdb][basic]"

typedef struct _env_list {
    char *key;
} env_list;

Device_Info Global_Device={0};

struct fdb_kvdb kvdb = { 0 };

rt_spi_flash_device_t fm25q128;
//void kvdb_basic_sample(fdb_kvdb_t kvdb)
//{
//    struct fdb_blob blob;
//    int boot_count = 0;
//
//    FDB_INFO("==================== kvdb_basic_sample ====================\n");
//
//    { /* GET the KV value */
//        /* get the "boot_count" KV value */
//        fdb_kv_get_blob(kvdb, "boot_count", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
//        /* the blob.saved.len is more than 0 when get the value successful */
//        if (blob.saved.len > 0) {
//            FDB_INFO("get the 'boot_count' value is %d\n", boot_count);
//        } else {
//            FDB_INFO("get the 'boot_count' failed\n");
//        }
//    }
//
//    { /* CHANGE the KV value */
//        /* increase the boot count */
//        boot_count ++;
//        /* change the "boot_count" KV's value */
//        fdb_kv_set_blob(kvdb, "boot_count", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
//        FDB_INFO("set the 'boot_count' value to %d\n", boot_count);
//    }
//
//    FDB_INFO("===========================================================\n");
//}
static void lock(fdb_db_t db)
{
    __disable_irq();
}

static void unlock(fdb_db_t db)
{
    __enable_irq();
}
static struct fdb_default_kv_node default_kv_table[] = {
    {"username", "armink", 0}, /* string KV */
    {"password", "123456", 0}, /* string KV */
};
int flash_Init(void)
{
    fdb_err_t result;
    rt_hw_spi_device_attach("spi2", "spi20", GPIOB, GPIO_PIN_12);//往总线spi2上挂载一个spi20设备，cs脚：PB12
    fm25q128 = rt_sfud_flash_probe("norflash0", "spi20");//使用sfud探测spi20从设备，并且将spi20连接的flash初始化为块设备，名称为FAL_USING_NOR_FLASH_DEV_NAME
    if (RT_NULL == fm25q128)
    {
        return -RT_ERROR;
    };

    struct fdb_default_kv default_kv;

    default_kv.kvs = default_kv_table;
    default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
    /* set the lock and unlock function if you want */
    //fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
    //fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);

    result = fdb_kvdb_init(&kvdb, "env", "fdb_kvdb1", &default_kv, NULL);

    if (result != FDB_NO_ERR) {
        return RT_ERROR;
    }
    //kvdb_basic_sample(&kvdb);
    return RT_EOK;

}
//void Boot_Times_Record(void)
//{
//    uint32_t i_boot_times = 0;
//    char *c_old_boot_times, c_new_boot_times[11] = {0};
//
//    /* 从环境变量中获取启动次数 */
//    c_old_boot_times = ef_get_env("boot_times");
//    /* 获取启动次数是否失败 */
//    if (c_old_boot_times == RT_NULL)
//        c_old_boot_times[0] = '0';
//
//    i_boot_times = atol(c_old_boot_times);
//    /* 启动次数加 1 */
//    i_boot_times++;
//    LOG_D("===============================================");
//    LOG_D("The system now boot %d times", i_boot_times);
//    LOG_D("===============================================");
//    /* 数字转字符串 */
//    sprintf(c_new_boot_times, "%ld", i_boot_times);
//    /* 保存开机次数的值 */
//    ef_set_env("boot_times", c_new_boot_times);
//}
//uint32_t Flash_Get_Boot_Times(void)
//{
//    const char *keybuf="boot_times";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
//    return read_value;
//}
//MSH_CMD_EXPORT(Flash_Get_Boot_Times,Flash_Get_Boot_Times);
uint32_t Flash_Get_Learn_Nums(void)
{
//    const char *keybuf="Learn_Nums";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
//    return read_value;

    struct fdb_blob blob;
    int value = 0;
    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&kvdb, "Learn_Nums", fdb_blob_make(&blob, &value, sizeof(value)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the 'Learn_Nums' value is %d\n", value);
        return value;
    } else {
        FDB_INFO("get the 'Learn_Nums' failed\n");
        return 0;
    }
}
uint32_t Flash_Get_Door_Nums(void)
{
//    char *keybuf = rt_malloc(20);
//    sprintf(keybuf, "%d", 88888888);//将传入的数字转换成数组
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
//    return read_value;

    struct fdb_blob blob;
    int value = 0;
    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&kvdb, "88888888", fdb_blob_make(&blob, &value, sizeof(value)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the 'Door_Num' value is %d\n", value);
        return value;
    } else {
        FDB_INFO("get the 'Door_Num' failed\n");
        return 0;
    }
}
uint32_t Flash_Get_Moto_Flag(void)
{
//    const char *keybuf="Moto";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Key %s value %ld \r\n", keybuf, read_value);//输出
//    return read_value;
    struct fdb_blob blob;
    int value = 0;
    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&kvdb, "Moto", fdb_blob_make(&blob, &value, sizeof(value)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the 'Moto' value is %d\n", value);
        return value;
    } else {
        FDB_INFO("get the 'Moto' failed\n");
        return 0;
    }
}
uint32_t Flash_Get_Key_Value(uint32_t key)
{
    char *keybuf = rt_malloc(20);
    sprintf(keybuf, "%ld", key);//将传入的数字转换成数组
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(keybuf);//释放临时buffer对应内存空间
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Key %d value %ld \r\n", key, read_value);//输出
//    return read_value;
    struct fdb_blob blob;
    int value = 0;
    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&kvdb, keybuf, fdb_blob_make(&blob, &value, sizeof(value)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the '%s' value is %d\n", keybuf,value);
        rt_free(keybuf);
        return value;
    } else {
        FDB_INFO("get the '%s failed\n",keybuf);
        rt_free(keybuf);
        return 0;
    }
}
void Flash_Key_Change(uint32_t key,uint32_t value)
{
    struct fdb_blob blob;
    char *Temp_KeyBuf = rt_malloc(20);
    sprintf(Temp_KeyBuf, "%ld", key);
//    char *Temp_ValueBuf = rt_malloc(20);//申请临时buffer空间
//    sprintf(Temp_ValueBuf, "%ld", value);
    fdb_kv_set_blob(&kvdb, Temp_KeyBuf, fdb_blob_make(&blob, &value, sizeof(value)));
    //ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
    rt_free(Temp_KeyBuf);
    //rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key %ld \r\n", value,key);
}
void Flash_LearnNums_Change(uint32_t value)
{
    struct fdb_blob blob;
    //const char *keybuf="Learn_Nums";
    //char *Temp_ValueBuf = rt_malloc(10);
    //sprintf(Temp_ValueBuf, "%ld", value);
    //ef_set_env(keybuf, Temp_ValueBuf);
    fdb_kv_set_blob(&kvdb, "Learn_Nums", fdb_blob_make(&blob, &value, sizeof(value)));
    //rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key Learn_Nums\r\n", value);
}
void Flash_Moto_Change(uint8_t value)
{
    struct fdb_blob blob;
//    char *keybuf="Moto";
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%d", value);
    LOG_D("Start write\r\n");
    fdb_kv_set_blob(&kvdb, "Moto", fdb_blob_make(&blob, &value, sizeof(value)));
    //ef_set_env(keybuf, Temp_ValueBuf);
    //rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key Moto\r\n", value);
}
//void Flash_Moto1Total_Add(void)
//{
//    char *keybuf="Moto1Total";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    LOG_I("Reading key %s is %d\r\n", keybuf,read_value);
//
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%ld", read_value+=1);
//    ef_set_env(keybuf, Temp_ValueBuf);
//    rt_free(Temp_ValueBuf);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Writing %ld to key %s\r\n", read_value,keybuf);
//}
//void Flash_Moto1Success_Add(void)
//{
//    char *keybuf="Moto1Success";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    LOG_I("Reading key %s is %d\r\n", keybuf,read_value);
//
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%ld", read_value+=1);
//    ef_set_env(keybuf, Temp_ValueBuf);
//    rt_free(Temp_ValueBuf);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Writing %ld to key %s\r\n", read_value,keybuf);
//}
//void Flash_Moto2Total_Add(void)
//{
//    char *keybuf="Moto2Total";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    LOG_I("Reading key %s is %d\r\n", keybuf,read_value);
//
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%ld", read_value+=1);
//    ef_set_env(keybuf, Temp_ValueBuf);
//    rt_free(Temp_ValueBuf);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Writing %ld to key %s\r\n", read_value,keybuf);
//}
//void Flash_Moto2Success_Add(void)
//{
//    char *keybuf="Moto2Success";
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    LOG_I("Reading key %s is %d\r\n", keybuf,read_value);
//
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%ld", read_value+=1);
//    ef_set_env(keybuf, Temp_ValueBuf);
//    rt_free(Temp_ValueBuf);
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Writing %ld to key %s\r\n", read_value,keybuf);
//}
uint8_t Device_RssiGet(uint32_t Device_ID)
{
    struct fdb_blob blob;
    char *keybuf = rt_malloc(20);
    sprintf(keybuf, "Rssi:%ld", Device_ID);//将传入的数字转换成数组
    int value = 0;
    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&kvdb, keybuf, fdb_blob_make(&blob, &value, sizeof(value)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the '%s' value is %d\n", keybuf,value);
        return value;
    } else {
        FDB_INFO("get the '%s' failed\n",keybuf);
        return 0;
    }
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(keybuf);//释放临时buffer对应内存空间
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Rssi %d value %ld \r\n", Device_ID, read_value);//输出
//    return read_value;
}
void Device_RssiChange(uint32_t Device_ID,uint8_t value)
{
    char *Temp_KeyBuf = rt_malloc(20);
//    char *Temp_ValueBuf = rt_malloc(20);
    sprintf(Temp_KeyBuf, "Rssi:%ld", Device_ID);
//    sprintf(Temp_ValueBuf, "%d", value);
//    ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
//    rt_free(Temp_KeyBuf);
//    rt_free(Temp_ValueBuf);
//    LOG_D("Writing Rssi %d to key %ld\r\n", value,Device_ID);
    struct fdb_blob blob;
//    char *keybuf="Moto";
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%d", value);
    fdb_kv_set_blob(&kvdb, Temp_KeyBuf, fdb_blob_make(&blob, &value, sizeof(value)));
    //ef_set_env(keybuf, Temp_ValueBuf);
    //rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key %s\r\n", value,Temp_KeyBuf);
}
uint8_t Device_BatGet(uint32_t Device_ID)
{
//    char *keybuf = rt_malloc(20);
//    sprintf(keybuf, "Bat:%ld", Device_ID);//将传入的数字转换成数组
//    char *read_value_temp;//真实值
//    uint32_t read_value = 0;
//    read_value_temp = strdup(ef_get_env(keybuf));
//    read_value = atol(read_value_temp);
//    rt_free(keybuf);//释放临时buffer对应内存空间
//    rt_free(read_value_temp);//释放临时buffer对应内存空间
//    LOG_D("Reading Bat %d value %ld \r\n", Device_ID, read_value);//输出
//    return read_value;
    struct fdb_blob blob;
    char *keybuf = rt_malloc(20);
    sprintf(keybuf, "Bat:%ld", Device_ID);//将传入的数字转换成数组
    int value = 0;
    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&kvdb, keybuf, fdb_blob_make(&blob, &value, sizeof(value)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the '%s' value is %d\n", keybuf,value);
        return value;
    } else {
        FDB_INFO("get the '%s' failed\n",keybuf);
        return 0;
    }
}
void Device_BatChange(uint32_t Device_ID,uint8_t value)
{
    char *Temp_KeyBuf = rt_malloc(20);
//    char *Temp_ValueBuf = rt_malloc(20);
    sprintf(Temp_KeyBuf, "Bat:%ld", Device_ID);
//    sprintf(Temp_ValueBuf, "%d", value);
//    ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
//    rt_free(Temp_KeyBuf);
//    rt_free(Temp_ValueBuf);
//    LOG_D("Writing Rssi %d to key %ld\r\n", value,Device_ID);
    struct fdb_blob blob;
//    char *keybuf="Moto";
//    char *Temp_ValueBuf = rt_malloc(10);
//    sprintf(Temp_ValueBuf, "%d", value);
    fdb_kv_set_blob(&kvdb, Temp_KeyBuf, fdb_blob_make(&blob, &value, sizeof(value)));
    //ef_set_env(keybuf, Temp_ValueBuf);
    //rt_free(Temp_ValueBuf);
    LOG_D("Writing %ld to key %s\r\n", value,Temp_KeyBuf);
//    char *Temp_KeyBuf = rt_malloc(20);
//    char *Temp_ValueBuf = rt_malloc(20);
//    sprintf(Temp_KeyBuf, "Bat:%ld", Device_ID);
//    sprintf(Temp_ValueBuf, "%d", value);
//    ef_set_env(Temp_KeyBuf, Temp_ValueBuf);
//    rt_free(Temp_KeyBuf);
//    rt_free(Temp_ValueBuf);
//    LOG_D("Writing Bat %ld to key %d\r\n", value,Device_ID);
}
uint8_t Add_Device(uint32_t Device_ID)
{
    uint32_t Num=0;
    Num = Flash_Get_Learn_Nums();
    if(Num>200)return RT_ERROR;
    Num++;
    Flash_LearnNums_Change(Num);
    Global_Device.Num = Num;
    Global_Device.ID[Num] = Device_ID;
    Flash_Key_Change(Num,Device_ID);
    return RT_EOK;
}
uint8_t Add_DoorDevice(uint32_t Device_ID)
{
    uint32_t Num=0;
    if(GetDoorID())
    {
        Num = Flash_Get_Door_Nums();
        Global_Device.ID[Num] = Device_ID;
        Flash_Key_Change(Num,Device_ID);
        Global_Device.DoorID = Num;
        Flash_Key_Change(88888888,Num);
        LOG_D("Replace Learn\r\n");
        return RT_EOK;
    }
    else
    {
        Num = Flash_Get_Learn_Nums();
        if(Num>200)return RT_ERROR;
        Num++;
        Flash_LearnNums_Change(Num);
        Global_Device.Num = Num;
        Global_Device.ID[Num] = Device_ID;
        Flash_Key_Change(Num,Device_ID);
        Global_Device.DoorID = Num;
        Flash_Key_Change(88888888,Num);
        LOG_D("New Learn\r\n");
        return RT_EOK;
    }
}
uint32_t GetDoorID(void)
{
    if(Global_Device.DoorID)
    {
        if(Global_Device.ID[Global_Device.DoorID])
        {
            return Global_Device.ID[Global_Device.DoorID];
        }
        else {
            LOG_D("Not Include Door Device\r\n");
            return 0;
        }
    }
    else
    {
        LOG_D("Not Include Door Device ID\r\n");
        return 0;
    }
}
uint8_t Delete_Device(uint32_t Num)
{
    Flash_Key_Change(Num,0);
    return RT_EOK;
}
uint8_t Update_Device_Bat(uint32_t Device_ID,uint8_t bat)//更新电量
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.Bat[num] = bat;
            Device_BatChange(Device_ID,bat);
            LOG_D("Device Bat %d is Increase to %d",Global_Device.ID[num],bat);
            return RT_EOK;
        }
        num--;
    }
    LOG_D("Device %d is Not Increase Success",Global_Device.ID[num]);
    return RT_ERROR;
}
uint8_t Update_Device_Rssi(uint32_t Device_ID,uint8_t rssi)//更新Rssi
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.Rssi[num] = rssi;
            Device_RssiChange(Device_ID,rssi);
            LOG_D("Device rssi %d is Increase to %d",Global_Device.ID[num],rssi);
            return RT_EOK;
        }
        num--;
    }
    LOG_D("Device %d is Not Increase Success",Global_Device.ID[num]);
    return RT_ERROR;
}
uint8_t Clear_Device_Time(uint32_t Device_ID)//更新时间戳为0
{
    uint16_t num = Global_Device.Num;
    LOG_D("Clear_Device_Time Num is %d\r\n",num);
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.ID_Time[num] = 0;
            Global_Device.Alive[num] = 1;
            LOG_D("Device %d is Clear to 0",Global_Device.ID[num]);
            return RT_EOK;
        }
        num--;
    }
    LOG_D("Device %d is Not Found\r\n",Device_ID);
    return RT_ERROR;
}
void Update_All_Time(void)
{
    uint16_t Num = Global_Device.Num;
    if(!Num)return;
    uint32_t Time = 0;
    for(uint8_t i=1;i<=Num;i++)
    {
        Time = Global_Device.ID_Time[i];//查询剩余时间
        if(Time<25)Time++;                     //自增
        Global_Device.ID_Time[i] = Time;//更新内存中的时间
        LOG_D("Device ID %ld With Time is Update to %d\r\n",Global_Device.ID[i],Global_Device.ID_Time[i]);
    }
    LOG_D("Update_All_Time OK\r\n");
}
void Clear_All_Time(void)
{
    uint16_t Num = Global_Device.Num;
    if(Num)
    {
        for(uint8_t i=1;i<=Num;i++)
        {
            if(Global_Device.ID_Time[i]<25)
            {
                Global_Device.ID_Time[i] = 0;//更新内存中的时间
                Global_Device.Alive[i] = 0;//更新内存中的时间
                LOG_D("Device %ld's time is cleard",Global_Device.ID[i]);
            }
        }
        LOG_D("Clear_All_Time OK\r\n");
    }
}
void Detect_All_Time(void)
{
    uint16_t num = Global_Device.Num;
    uint8_t WarnFlag = 0;
    if(!num)return;
    while(num)
    {
        if(Global_Device.ID_Time[num]==25 && Global_Device.Alive==0)
        {
            WarnFlag = 1;
            //掉线ID上报
            if(Global_Device.ID[num] == Global_Device.DoorID)
            {
                LOG_D("Door is Offline\r\n");
            }
            else
            {
                LOG_D("Device ID %ld is Offline\r\n",Global_Device.ID[num]);
            }
        }
        num--;
    }
    if(WarnFlag)
    {
        Warning_Enable_Num(5);
    }
    Clear_All_Time();
    LOG_D("Detect_All_Time OK\r\n");
}
uint8_t Remote_Delete(uint32_t Device_ID)
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            Global_Device.ID[num] = 0;
            Delete_Device(num);
            LOG_I("Delete ID %ld By WIFI is success\r\n");
            return RT_EOK;
        }
        num--;
    }
    LOG_I("Delete ID %ld By WIFI is fail\r\n");
    return RT_ERROR;
}
uint8_t Flash_Get_Key_Valid(uint32_t Device_ID)//查询内存中的ID
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)return RT_EOK;
        num--;
    }
    return RT_ERROR;
}
uint8_t Flash_GetRssi(uint32_t Device_ID)//查询内存中的RSSI
{
    uint16_t num = Global_Device.Num;
    if(!num)return RT_ERROR;
    while(num)
    {
        if(Global_Device.ID[num]==Device_ID)
        {
            return Global_Device.Rssi[num];
        }
        num--;
    }
    return 0;
}
void Detect_All_TimeInDecoder(uint8_t ID)
{
    if(Flash_Get_Key_Valid(ID)==RT_EOK)
    {
        Clear_Device_Time(ID);
    }
    uint16_t num = Global_Device.Num;
    uint8_t WarnFlag = 0;
    if(!num)return;
    while(num)
    {
        if(Global_Device.ID_Time[num]==25 && Global_Device.Alive[num]==0)
        {
            WarnFlag = 1;
            //掉线ID上报
            if(Global_Device.ID[num] == Global_Device.DoorID)
            {
                LOG_D("Door is Offline\r\n");
            }
            else
            {
                LOG_D("Device ID %ld is Offline\r\n",Global_Device.ID[num]);
            }
        }
        num--;
    }
    if(WarnFlag==0)
    {
        OfflineDisableWarning();
    }
    LOG_D("Detect_All_Time OK\r\n");
}

void LoadDevice2Memory(void)//数据载入到内存中
{
    memset(&Global_Device,0,sizeof(Global_Device));
    Global_Device.Num = Flash_Get_Learn_Nums();
    LOG_D("num is %d",Global_Device.Num);
//    for(uint8_t i=1;i<=Global_Device.Num;i++)
//    {
//        Global_Device.ID[i] = Flash_Get_Key_Value(i);
//        LOG_D("GOT ID is %ld\r\n",Global_Device.ID[i]);
//        Global_Device.Bat[i] = Device_BatGet(Global_Device.ID[i]);
//        LOG_D("GOT Bat is %ld\r\n",Global_Device.Bat[i]);
//        Global_Device.Rssi[i] = Device_RssiGet(Global_Device.ID[i]);
//        LOG_D("GOT Rssi is %ld\r\n",Global_Device.Rssi[i]);
//    }
    Global_Device.DoorID = Flash_Get_Key_Value(88888888);
    Global_Device.LastFlag = Flash_Get_Moto_Flag();
}
MSH_CMD_EXPORT(LoadDevice2Memory,LoadDevice2Memory);
void DeleteAllDevice(void)//数据载入到内存中
{
    LOG_D("Before Delete num is %d",Global_Device.Num);
    memset(&Global_Device,0,sizeof(Global_Device));
    Flash_LearnNums_Change(0);
    Flash_Key_Change(88888888,0);//DoorID
    Flash_Moto_Change(0);//LastFlag
    LOG_D("After Delete num is %d",Global_Device.Num);
}
MSH_CMD_EXPORT(DeleteAllDevice,DeleteAllDevice);
