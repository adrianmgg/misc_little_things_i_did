approx. started: 2021/10/01
approx. finished: 2021/10/02

**WARNING - this is not complete and parts of it have not been tested**

allows you to write something like this:
```typescript
const foo = _checktype_hasallkeys_factory({
	required: {
		a: _checktype_string,
		b: _checktype_number,
		c: _checktype_union_factory(
			_checktype_literal_factory(32 as const),
			_checktype_arrayof_factory(_checktype_number)
		),
	},
	optional: {
		d: _checktype_tuple_factory(
			_checktype_number,
			_checktype_literal_factory(12 as const),
			_checktype_null
		),
	}
});
```

which gives you a type guard function with this type 
```typescript
const foo: (a: unknown) => a is {
    a: string;
    b: number;
    c: 32 | number[];
    d?: [number, 12, null] | undefined;
}
```

all the \_checktype functions are just normal type guard functions, so any language-provided (like `Array.isArray`) or user-defined one should work fine with all the functions like `_checktype_tuple_factory`, `_checktype_arrayof_factory`, `_checktype_union_factory`, etc. 
