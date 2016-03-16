/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"




/***********************************************************************
* get size of a context dma object
***********************************************************************/
S32 rsx_object_context_dma_get_object_size() {
    return 0x10;
}




/***********************************************************************
* 
***********************************************************************/
void rsx_object_context_dma_220064(rsx_ctx_dma_obj_t* dma_obj, S32 arg1, S32 arg2, S64 addr, S32 size) {
    S32 value1 = 0, value2 = 0, offset = 0;
    rsx_dev_core_obj_t* core = NULL;
    
    
    // ? based on arg1
    if (arg1 == 1)
      value1 = 0x3002;
    else if (arg1 == 2)
      value1 = 0x3003;
    else if (arg1 == 3)
      value1 = 0x303D;
    else {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
  }
  
  // get RSX device core object
  core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  
    // ? based on arg2
    if (arg2 == 4) {
        addr = rsx_core_memory_get_BAR1_offset_by_address((void*)core->core_mem_obj, addr);
        value2 = (addr <<20) | value1;
    }
    else if (arg2 == 8)
        value2 = (value1 | 0x20000) | (addr <<20);
    else {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
  }
    
    // ? alignment check
    if ((addr &= 0xF) != 0) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
  }
    
    // ?
    if (size != 0)
      size--;
    else
      size = 0;
    
    // store address into dma object
    dma_obj->unk_10 = (S32)addr;
    
    // get entry offset
    offset = rsx_core_memory_2120EC((void*)core->core_mem_obj, dma_obj->unk_08);
    
    // write entry
    DDR_write32(  value2, offset + g_rsx_bar2_addr);
    DDR_write32(    size, offset + g_rsx_bar2_addr + 4);
    DDR_write32(addr | 3, offset + g_rsx_bar2_addr + 8);
    DDR_write32(addr | 3, offset + g_rsx_bar2_addr + 0xC);
    
    //printf("addr: 0x%016llX\n", offset + g_rsx_bar2_addr);
    return;
}

/***********************************************************************
* 
***********************************************************************/
rsx_ctx_dma_obj_t* rsx_object_context_dma_create_obj(U32 type) {
    S32 ret = -1;
    S64 idx;
    rsx_dev_core_obj_t* core = NULL;
    rsx_core_mem_obj_t* core_mem = NULL;
    rsx_utils_bm_obj_t* bm_ctx_dma = NULL;
    rsx_ctx_dma_obj_t* dma_obj = NULL;
    
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
  
  // get core memory object
  core_mem = (void*)core->core_mem_obj;
  
  // get context DMA object bitmap
  bm_ctx_dma = (void*)core_mem->bm_ctx_dma;
  
  // allocate 1 of the 256 context DMA objects
    ret = rsx_utils_bitmap_allocate(bm_ctx_dma, 1, &idx);
    if (ret == 0)  // allocation fails {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // allocate object
    dma_obj = lv1_kmalloc(sizeof(rsx_ctx_dma_obj_t));
    if (dma_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
    }
    
    // init dma object
    dma_obj->type   = type;
    dma_obj->unk_10 = -1;
    dma_obj->unk_08 = idx;
    
    return dma_obj;
}