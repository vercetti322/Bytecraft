program ArrayAverage;
var
  numbers: array[1..5] of Integer;
  i, sum : Integer;
  average : real;
begin
  write("Enter 5 integer values:");
  for i := 1 to 5 do
  begin
    read(numbers[i]);
  end;
  sum := 0;
  for i := 1 to 5 do
  begin
    sum := sum + numbers[i];
  end;
  average := sum / 5;
  write("The sum and average are: "); 
  write(sum, average);
end.