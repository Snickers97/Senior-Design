for(i = 0; 2*i < size; i++){
  audio[2*i]=(val>>8);
  audio[(2*i)+1]=val;
}


//above for some uint16_t 'val'
//      for some uint8_t array 'audio'
