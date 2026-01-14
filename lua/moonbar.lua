--- TODO(@s0cks): clean this up, weird bindings naming
local M = CoreLib

M.inspect = require('inspect')

local function split(s, sep)
  local values = {}
  local pattern = "[^" .. sep .. "]+"
  for match in s:gmatch(pattern) do
    table.insert(values, match)
  end
  return values
end

local function print_path_var(name, path)
  print('  ' .. name .. ':')
  for idx, p in ipairs(split(path, ';')) do
    print('    - ' .. p)
  end
end

function M.print_info()
  print('')
  print('moonbar info:')
  --- TODO(@s0cks): print version
  print('  config dir: ' .. M.get_config_dir())
  print('  current working dir: ' .. M.get_cwd())
  print_path_var('path', package.path)
  print_path_var('cpath', package.cpath)
  print('')
end

if M.is_debug() then
  M.print_info()
end

return M
