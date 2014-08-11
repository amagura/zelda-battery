

function getPowerLevel(cmd, args)
  local child = io.popen(cmd)
  return child:read("*all")
end
