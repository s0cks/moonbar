local M = {}

M.Label = LabelLib
M.Button = ButtonLib

local Box = BoxLib
M.Box = Box

local HORIZONTAL = 0
function Box.new_horizontal()
  return M.new(HORIZONTAL)
end

local VERTICAL = 1
function Box.new_vertical()
  return M.new(VERTICAL)
end

return M
