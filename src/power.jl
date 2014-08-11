function onAcPower(util, args)
  chomp(readall(`$util $args`));
end

onAcPower("pmset", "-g ps")
