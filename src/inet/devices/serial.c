

//bool serial_read_to_size(size_t size);
//void serial_write(void *buf, size_t size);





/*bool serial_read_to_size(size_t size){
    static size_t bytes_read = 0;
    bytes_read += srl_Read(&srl, &net_parse_buffer[bytes_read], size - bytes_read);
    if(bytes_read >= size) {bytes_read = 0; return true;}
    else return false;
}

void serial_write(void *buf, size_t size) {
    srl_Write(&srl, buf, size);
}
*/


