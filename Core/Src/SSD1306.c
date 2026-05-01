#include <SSD1306.h>
#include <FontSSD1306.h>
void Oled_SendCmd(uint8_t cmd){
    HAL_I2C_Mem_Write(&hi2c2, Oled_Adr, Oled_Control, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100  );
}

void Oled_SendData(uint8_t data){
    HAL_I2C_Mem_Write(&hi2c2, Oled_Adr, Oled_Data, I2C_MEMADD_SIZE_8BIT, &data, 1, 100  );
}
void OLED_Init(){
    // fundelmental init
	HAL_Delay(100);
    Oled_SendCmd(0xAE); // khoi tao man hinh off
		
		Oled_SendCmd(0xA0); // Trả trục X về bình thường
		Oled_SendCmd(0xC0); // Trả trục Y về bình thường4  
		Oled_SendCmd(0xA8); // Set Multiplex Ratio
    Oled_SendCmd(0x3F);
    Oled_SendCmd(0xD3); // Set Display Offset
    Oled_SendCmd(0x00);
		
    Oled_SendCmd(0xDA);
    Oled_SendCmd(0x12);
    Oled_SendCmd(0x8D); // Charge Pump 
    Oled_SendCmd(0x14); // Enable Charge Pump
	
    Oled_SendCmd(0x81);// chinh do sang man hinh
    Oled_SendCmd(0xFF);// do sang max

    Oled_SendCmd(0xA6); // che do hien thi binh thuong
    //timing 
    Oled_SendCmd(0xD5); // chinh fresh rate
    Oled_SendCmd(0x80); // fresh rate 128Hz
    
    //adress setting
    Oled_SendCmd(0x20); // chinh dia chi
    Oled_SendCmd(0x00); // horizontal addressing mode

    Oled_SendCmd(0xAF); // khoi tao man hinh on
}

void OLED_Yellow(){
    Oled_SendCmd(0x22); // chinh page trong khoang vung vang
    Oled_SendCmd(0x00); // page start
    Oled_SendCmd(0x01); // page end
}

void OLED_Blue(){
    Oled_SendCmd(0x22); // chinh page trong khoang vung xanh
    Oled_SendCmd(0x02); // page start
    Oled_SendCmd(0x07); // page end
}


void OLED_Cusor(uint8_t x, uint8_t y){
    Oled_SendCmd(0x21); // chinh column
    Oled_SendCmd(x); // column start
    Oled_SendCmd(127); // column end

    Oled_SendCmd(0x22); // chinh page
    Oled_SendCmd(y); // page start
    Oled_SendCmd(7); // page end
}

void OLED_Write(char c, uint8_t x, uint8_t y){
    OLED_Cusor(x, y);
    int i;
    if (c >= 'A' && c <= 'Z') {
        for (i =0; i< 5; i++){
             const uint8_t* charData = Font5x8_Alphabet[c - 'A'] + i;
             Oled_SendData(*charData);
        }
    }else if( c>= 'a' && c <='z'){
        for (i =0; i< 5; i++){
             const uint8_t* charData = Font5x8_Alphabet_koviethoa[c - 'a'] + i;
             Oled_SendData(*charData);
        }
    }else if( c>= '0' && c<='9'){
        for (i =0; i< 5; i++){
             const uint8_t* charData = Font5x8_Number[c - '0'] + i;
             Oled_SendData(*charData);
        }
			}
		else {
						int symbol_idx = -1; 
						switch (c) {
								case '!': symbol_idx = 0;  break;
								case '"': symbol_idx = 1;  break;
								case '#': symbol_idx = 2;  break;
								case '$': symbol_idx = 3;  break;
								case '%': symbol_idx = 4;  break;
								case '&': symbol_idx = 5;  break;
								case '\'':symbol_idx = 6;  break;
								case '(': symbol_idx = 7;  break;
								case ')': symbol_idx = 8;  break;
								case '*': symbol_idx = 9;  break;
								case ':': symbol_idx = 10; break;
								case '/': symbol_idx = 11; break;
								case ' ':
										for (i = 0; i < 5; i++) Oled_SendData(0x00);
										return;
						}

						if (symbol_idx != -1) {
								for (i = 0; i < 5; i++) {
										Oled_SendData(Font5x8_Symbol[symbol_idx][i]);
								}
						}
    }

    Oled_SendData(0x00); // them 1 cot trang sau khi in 1 ky tu
}

void OLED_Print(char* str, uint8_t x, uint8_t y){
    while(*str){
        OLED_Write(*str, x, y);
        str++;
        x += 6; // chuyen sang cot tiep theo (5 cot ky tu + 1 cot trang)

        if( x> 127){ // neu vuot qua cot cuoi cung thi chuyen sang dong tiep theo
            x = 0;
            y++;
        }
    }
}
void OLED_Clear() {
    Oled_SendCmd(0x21); // Reset cột về 0-127
    Oled_SendCmd(0);
    Oled_SendCmd(127);
    Oled_SendCmd(0x22); // Reset page về 0-7
    Oled_SendCmd(0);
    Oled_SendCmd(7);

    for (uint16_t i = 0; i < 1024; i++) {
        Oled_SendData(0x00);
    }
}

void OLED_DrawPixel(uint8_t x, uint8_t y)
{
    if(x>128  || y<64)
        return;
    uint8_t page=y/8; //tính page
    uint8_t bit_posion=y&8;//vị trí hàng thứ y trong page
    uint8_t data=1<<bit_posion; //tạo dữ liệu để bật bit tương ứng
    OLED_Cusor(x, page); //đặt con trỏ đến vị trí cần vẽ
    Oled_SendData(data); //gửi dữ liệu để bật pixel
}

void OLED_DrawCircle(uint8_t x0, uint8_t y0, uint8_t r)
{
    uint8_t x=r;
    uint8_t y=0;
    int8_t err=0;
    while(x>=y)
    {
        OLED_DrawPixel(x0+x, y0+y);
        OLED_DrawPixel(x0+y, y0+x);
        OLED_DrawPixel(x0-y, y0+x);
        OLED_DrawPixel(x0-x, y0+y);
        OLED_DrawPixel(x0-x, y0-y);
        OLED_DrawPixel(x0-y, y0-x);
        OLED_DrawPixel(x0+y, y0-x);
        OLED_DrawPixel(x0+x, y0-y);

        if(err<=0)
        {
            y++;
            err+=2*y+1;
        }
        if(err>0)
        {
            x--;
            err-=2*x+1;
        }
    }
}

void OLED_DrawSlectionBox(uint8_t x0, uint8_t y0, uint8_t width){
    for(uint8_t i=0; i<width; i++){
        OLED_DrawPixel(x0+i, y0); // Vẽ đường ngang trên
        OLED_DrawPixel(x0+i, y0+7); // Vẽ đường ngang dưới
    }
    for(uint8_t j=0; j<8; j++){
        OLED_DrawPixel(x0, y0+j); // Vẽ đường dọc trái
        OLED_DrawPixel(x0+width-1, y0+j); // Vẽ đường dọc phải
    }
}

