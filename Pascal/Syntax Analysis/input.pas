program NumberOfDigits;
var
  number, count: Integer;
  flag : boolean; 
begin
  write("Enter a number:");
  read(number);
  count := 0;
  while number <> 0 do
  begin
    if flag  then
    begin
      count:=count+1;
      flag := 1;
    end
    else 
    begin
     count :=count;
    end; 
    number := number / 10;
  end;
  write("The number of digits is: ");
  write(count);
end.
