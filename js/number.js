$('input.number').keyup(function(event) {

  // skip for arrow keys
  if(event.which >= 37 && event.which <= 40) return;

  // format number
  $(this).val(function(index, value) {
    value = value.replace(/(?!-)[^0-9.]/g, "");
    valueArray = value.split('.');
    if(valueArray.length >= 2 ){
        return valueArray[0].replace(/\B(?=(\d{3})+(?!\d))/g, ",")+'.'+valueArray[1];
    }else{
        return valueArray[0].replace(/\B(?=(\d{3})+(?!\d))/g, ",")
    } 

    return value[0].replace(/\B(?=(\d{3})+(?!\d))/g, ",")
    ;
  });
});