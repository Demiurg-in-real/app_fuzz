#include <stdint.h>
#include<stdio.h>
static void GOST_Magma_g(const uint8_t *k, const uint8_t *a, uint8_t *out_data) 
{ 
	uint8_t internal[4]; 
	uint32_t out_data_32; 
	// Складываем по модулю 32 правую половину блока с итерационным ключом 
	GOST_Magma_Add_32(a, k, internal); 
	// Производим нелинейное биективное преобразование результата 
	GOST_Magma_T(internal, internal); 
	// Преобразовываем четырехбайтный вектор в одно 32-битное число 
	out_data_32 = internal[0]; 
	out_data_32 = (out_data_32 << 8) + internal[1]; 
	out_data_32 = (out_data_32 << 8) + internal[2]; 
	out_data_32 = (out_data_32 << 8) + internal[3]; 
	// Циклически сдвигаем все влево на 11 разрядов 
	out_data_32 = (out_data_32 << 11)|(out_data_32 >> 21); 
	// Преобразовываем 32-битный результат сдвига обратно в 4-байтовый вектор 
	out_data[3] = out_data_32; 
	out_data[2] = out_data_32 >> 8; 
	out_data[1] = out_data_32 >> 16; 
	out_data[0] = out_data_32 >> 24; 
}
