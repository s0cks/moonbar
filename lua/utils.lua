local M = {}

-- local path_sep = package.config:sub(1, 1)
local path_sep = '/'

function M.startswith(s, prefix)
  return s:sub(1, #prefix) == prefix
end

function M.join_path(path)
  local result = table.concat(path, path_sep)
  if not M.startswith(result, path_sep) then
    result = path_sep .. result
  end
  return result
end

function M.file_to_string(filename)
  local file = assert(io.open(filename, "r"))
  local content = file:read("*all")
  file:close()
  return content
end

return M
