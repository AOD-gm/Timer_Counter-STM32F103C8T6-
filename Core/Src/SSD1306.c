#include <SSD1306.h>
#include <FontSSD1306.h>
void Oled_SendCmd(uint8_t cmd){
    HAL_I2C_Mem_Write(&hi2c1, Oled_Adr, Oled_Control, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100  );
}

void Oled_SendData(uint8_t data){
    HAL_I2C_Mem_Write(&hi2c1, Oled_Adr, Oled_Data, I2C_MEMADD_SIZE_8BIT, &data, 1, 100  );
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
    if(x>129  || y>65)
        return;
    uint8_t page=y/8; //tính page
    uint8_t bit_posion=y % 8;//vị trí hàng thứ y trong page
    uint8_t data=1<<bit_posion; //tạo dữ liệu để bật bit tương ứng
    OLED_Cusor(x, page); //đặt con trỏ đến vị trí cần vẽ
    Oled_SendData(data); //gửi dữ liệu để bật pixel
}

void OLED_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r)
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

void OLED_DrawSelectionBox(uint8_t x0, uint8_t y0, uint8_t width){
    uint8_t y_pixel = y0 * 8; // chuyển đổi page sang pixel
    uint8_t y_pixdel_end = y_pixel + 7; // tính pixel cuối cùng của page
    for(uint8_t i=0; i<=width; i++){
        OLED_DrawPixel(x0+i-1, y_pixel-2); // vẽ đường trên
        OLED_DrawPixel(x0+i-1, y_pixdel_end+2); // vẽ đường dưới
    }
    OLED_Cusor(x0-2, y0-1); // đặt con trỏ sau khi vẽ xong
    Oled_SendData(0xC0);
    OLED_Cusor(x0-2, y0); 
    Oled_SendData(0xFF); // thêm 1 cột trống sau khi vẽ xong
    OLED_Cusor(x0-2, y0+1);
    Oled_SendData(0x03); // thêm 1 cột trống sau khi vẽ xong;
    OLED_Cusor(x0+width, y0-1); // đặt con trỏ sau khi vẽ xong
    Oled_SendData(0xC0);
    OLED_Cusor(x0+width, y0);
    Oled_SendData(0xFF); // thêm 1 cột trống sau
    OLED_Cusor(x0+width, y0+1);
    Oled_SendData(0x03); // thêm 1 cột trống sau khi vẽ xong;
}

void OLED_DrawSelectionBracket(uint8_t x0, uint8_t y0, uint8_t width) {
    // Vẽ ngoặc mở "[" bên trái
    OLED_Cusor(x0 - 6, y0); 
    Oled_SendData(0xFF); // Vạch dọc dài
    OLED_Cusor(x0 - 5, y0); 
    Oled_SendData(0x81); // 2 chấm trên dưới
    
    // Vẽ ngoặc đóng "]" bên phải
    OLED_Cusor(x0 + width + 4, y0); 
    Oled_SendData(0x81); // 2 chấm trên dưới
    OLED_Cusor(x0 + width + 5, y0); 
    Oled_SendData(0xFF); // Vạch dọc dài
}

void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;
    for (;;) {
        OLED_DrawPixel(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void OLED_DrawHalfCircle(uint16_t x0, uint16_t y0, uint16_t r) {
    int x = r;
    int y = 0;
    int err = 0;
    while (x >= y) {
        OLED_DrawPixel(x0 - x, y0 - y);
        OLED_DrawPixel(x0 - y, y0 - x);
        OLED_DrawPixel(x0 + y, y0 - x);
        OLED_DrawPixel(x0 + x, y0 - y);
        if (err <= 0) {
            y++;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}