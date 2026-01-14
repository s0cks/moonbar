local M = {}
local utils = require('utils')

local POWER_SUPPLY_DIR = utils.join_path({
  "sys",
  "class",
  "power_supply",
})

local Battery = {}
Battery.__index = Battery

function Battery:get_dir()
  return utils.join_path({
    POWER_SUPPLY_DIR,
    self.name,
  })
end

local function read_battery_file(battery, filename)
  return utils.file_to_string(utils.join_path({
    battery:get_dir(),
    filename,
  }))
end

local battery_files = {
  'uevent',
  'charge_full_design',
  'serial_number',
  'eppid',
  'technology',
  'current_now',
  'charge_now',
  'present',
  'power',
  'manufacturer',
  'charge_control_start_threshold',
  'device',
  'type',
  'charge_full',
  'manufacture_year',
  'capacity',
  'manufacture_day',
  'health',
  'cycle_count',
  'voltage_now',
  'subsystem',
  'status',
  'alarm',
  'model_name',
  'temp',
  'charge_types',
  'voltage_min_design',
  'capacity_level',
  'manufacture_month',
  'charge_control_end_threshold',
  'hwmon1',
  'extensions',
  'runtime_active_time',
  'runtime_status',
  'autosuspend_delay_ms',
  'runtime_suspended_time',
  'control',
  'uevent',
  'in0_input',
  'power',
  'temp1_label',
  'device',
  'subsystem',
  'temp1_input',
  'curr1_input',
  'name',
  'dell-wmi-ddv',
  'runtime_active_time',
  'runtime_status',
  'autosuspend_delay_ms',
  'runtime_suspended_time',
  'control',
}

for _, name in ipairs(battery_files) do
  Battery[name] = function(battery)
    return read_battery_file(battery, name)
  end
end

function M.new(name)
  local value = {
    name = name or "BAT0",
  }
  setmetatable(value, Battery)
  return value
end

return M
