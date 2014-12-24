str="$(echo $(../zb-text))"
string="$(echo "$str" | sed -r 's/(.)/\1 /g; s/\s$//')"

full=0
empty=0
for char in $string; do
  [[ "$char" =~ "♥" ]] && ((++full))
  [[ "$char" =~ "♡" ]] && ((++empty))
done

echo "full: $full"
echo "empty: $empty"
