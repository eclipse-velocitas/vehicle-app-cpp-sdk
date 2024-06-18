## Usage

1) Create a .json file named: "subscription_signals.json"
2) Place it in the same folder as the binary
3) Exeucute the binary e.g. "./example-performance-subscribe"<br>
   Alternatively, you can specify the path of the signal list explicitly: "./example-performance-subscribe \<path-to-json\>"
4) Check the console output for the timestamps: "\<Timestamp\> - \<Signal_Name\> - \<Value\>"

## .json format

The following format is mandatory for the input file:

```
{
  "signals": [
    {
      "path": "Vehicle.LowVoltageBattery.CurrentVoltage"
    },
    {
      "path": "Vehicle.LowVoltageBattery.CurrentCurrent"
    },
    {
      "path": "Vehicle.Speed"
    },
    ...
  ]
}
```

Signals which are not available will be printed in the console as a warning.
