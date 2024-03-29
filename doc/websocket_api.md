# pyth-client websocket API
The pythd daemon supports a websocket interface based on the jsoni-rpc 2.0 standard.  Methods include:

- [get_product_list](#get_product_list)
- [update_price](#update_price)
- [subscribe_price](#subscribe_price)
- [subscribe_price_sched](#subscribe_price_sched)

Batch requests are processed in the order the requests appear within the batch.

## get_product_list

Get list of available symbols and associated meta data.

Request looks like:
```
{
  "jsonrpc": "2.0",
  "method": "get_product_list",
  "id" : 1
}
```

A successful response looks something like:
```
{
 "jsonrpc": "2.0",
 "result": [
  {
   "account": "9F6eBgAfktth93C9zmtKDXFXNjZkq6JwJR56VPKqWmJm",
   "attr_dict": {
    "symbol": "SYMBOL1/USD",
    "asset_type": "Equity",
    "country": "USA",
    "description": "pyth example product #1",
    "quote_currency": "USD",
    "tenor": "Spot",
    "cms_symbol": "SYMBOL1",
    "cqs_symbol": "SYMBOL1",
    "nasdaq_symbol": "SYMBOL1"
   },
   "price": [
    {
     "account": "CrZCEEt3awgkGLnVbsv45Pp4aLhr7fZfZr3ubzrbNXaq",
     "price_exponent": -4,
     "price_type": "price"
    }
   ]
  },
  {
   "account": "HCFaDYyz1ajS57HfCaaqzA1cZSaa2oEccQejbHaaofd4",
   "attr_dict": {
    "symbol": "SYMBOL2/USD",
    "asset_type": "Equity",
    "country": "USA",
    "description": "pyth example product #2",
    "quote_currency": "USD",
    "tenor": "Spot",
    "cms_symbol": "SYMBOL2",
    "cqs_symbol": "SYMBOL2",
    "nasdaq_symbol": "SYMBOL2"
   },
   "price": [
    {
     "account": "7FUsKvvtN5rB1fgYFWZLo5DLcqHTTeu63bUPThYT6MiS",
     "price_exponent": -4,
     "price_type": "price"
    }
   ]
  }
 ],
 "id": null
}
```

## update_price

Update component price of some symbol using the publishing key of the pythd daemon.

Request includes the pricing account from the get_product_list output and looks something like:
```
{
  "jsonrpc": "2.0",
  "method": "update_price",
  "params" : {
    "account": "CrZCEEt3awgkGLnVbsv45Pp4aLhr7fZfZr3ubzrbNXaq",
    "price" : 42002,
    "conf" : 3,
    "status": "trading"
  },
  "id" : 1
}
```

The price and confidence interval (conf) attributes are expressed as integers with an implied decimal point given by the the price_exponent defined by symbol. The price type is a string with one of the following values: "price" or "twap". The symbol status is a string with one of the following values: "trading" or "halted".

A successful response looks like:
```
{
  "jsonrpc": "2.0",
  "result" : 0,
  "id" : 1
}
```

## subscribe_price

Subscribe to symbol price updates.

Request looks like:

```
{
  "jsonrpc": "2.0",
  "method": "subscribe_price",
  "params" : {
    "account": "CrZCEEt3awgkGLnVbsv45Pp4aLhr7fZfZr3ubzrbNXaq",
  },
  "id" : 1
}
```

A successful response looks like:

```
{
  "jsonrpc": "2.0",
  "result" : {
    "subscription" : 1234
  },
  "id" : 1
}
```

The subscription identifier in the result is used in all subsequent notifications for this subscription. An example notification (with the same subscription id) looks like:


```
{
  "jsonrpc": "2.0",
  "method": "notify_price",
  "params": {
    "result": {
      "price" : 42002,
      "conf" : 3,
      "status" : "trading",
      "valid_slot" : 32008,
      "pub_slot" : 32009
    },
    "subscription" : 1234
  }
}
```

Results include the most recent aggregate price, confidence interval and symbol status. pythd will submit a `notify_price` message immediately upon subscription with the latest price instead of waiting for an update.

Results also include two slot numbers. `valid_slot` corresponds to the slot containing the prices that were used to compute the aggregate price. `pub_slot` corresponds to the slot in which the aggregation price was published.

## subscribe_price_sched

Subscribe to price update schedule. pythd will notify the client whenever it should submit the next price for a subscribed symbol.

Request looks like:

```
{
  "jsonrpc": "2.0",
  "method": "subscribe_price_sched",
  "params" : {
    "account": "CrZCEEt3awgkGLnVbsv45Pp4aLhr7fZfZr3ubzrbNXaq",
  },
  "id" : 1
}
```

A successful response looks like:

```
{
  "jsonrpc": "2.0",
  "result" : {
    "subscription" : 1234
  },
  "id" : 1
}
```

Where the result is an integer corresponding to a subscription identifier. All subsequent notifications for this subscription correspond to this identifier.

```
{
  "jsonrpc": "2.0",
  "method": "notify_price_sched",
  "params": {
    "subscription" : 1234
  }
}
```
