function _checktype_string(x: unknown): x is string {
	return typeof x === 'string';
}
function _checktype_number(x: unknown): x is number {
	return typeof x === 'number';
}
function _checktype_object(x: unknown): x is object {
	return typeof x === 'object' && x !== null && !Array.isArray(x);
}
function _checktype_array(x: unknown): x is unknown[] {
	return Array.isArray(x);
}
function _checktype_bool(x: unknown): x is boolean {
	return typeof x === 'boolean';
}

function _checktype_any(_x: unknown): _x is unknown {
	return true;
}

function _checktype_literal_factory<L>(l: L): (a: unknown) => a is L {
	return function(x): x is L {
		return x === l;
	}
}

const _checktype_null = _checktype_literal_factory(null);
const _checktype_undefined = _checktype_literal_factory(undefined);

type UnionOf<T extends unknown[]> =
	T extends [infer A1] ?
		A1 :
	T extends [infer A1, infer A2] ?
		A1 | A2 :
	T extends [infer A1, infer A2, infer A3, infer A4, infer A5, infer A6, infer A7, infer A8, ...infer Rest] ?
		A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8 | UnionOf<Rest> :
	T extends [infer A1, infer A2, infer A3, infer A4, ...infer Rest] ?
		A1| A2 | A3 | A4 | UnionOf<Rest> :
	T extends [infer A1, infer A2, ...infer Rest] ?
		A1 | A2 | UnionOf<Rest> :
	never;

// type UnionOf<T extends unknown[]> = T extends [infer First, ...infer Rest] ? First | UnionOf<Rest> : never;

type IntersectionOf<T extends unknown[]> =
	T extends [infer A1] ?
		A1 :
	T extends [infer A1, infer A2] ?
		A1 & A2 :
	T extends [infer A1, infer A2, infer A3, infer A4, infer A5, infer A6, infer A7, infer A8, ...infer Rest] ?
		A1 & A2 & A3 & A4 & A5 & A6 & A7 & A8 & IntersectionOf<Rest> :
	T extends [infer A1, infer A2, infer A3, infer A4, ...infer Rest] ?
		A1 & A2 & A3 & A4 & IntersectionOf<Rest> :
	T extends [infer A1, infer A2, ...infer Rest] ?
		A1 & A2 & IntersectionOf<Rest> :
	never;

// function _checktype_keys<T extends string[]>(x: unknown, ...keys: T): x is {
// 	[K in keyof T as (T[K] extends string ? T[K] : never)]: unknown
// } {
// 	if(!_checktype_object(x)) return false;
// 	for(const key of keys) {
// 		if(!(key in x)) return false;
// 	}
// 	return true;
// }

// function _checktype_keyval<
// 	T,
// 	CheckType extends T[Key],
// 	Key extends keyof T
// >(x: T, key: Key, checker: (a: unknown) => a is CheckType): x is {
// 	[K in keyof T]: K extends Key ? (CheckType extends T[K] ? CheckType : T[K]) : T[K]
// } {
// 	return checker(x[key]);
// }

// get the type which a type guard guards
type GuardType<T extends (a: unknown) => boolean> =
	T extends (b: unknown) => b is infer G ? G : never;

function _checktype_union_factory<
	CheckFuncs extends ((a: unknown) => boolean)[],
	UnionToCheck = UnionOf<{
		[K in keyof CheckFuncs]: CheckFuncs[K] extends (c: unknown) => boolean ? GuardType<CheckFuncs[K]> : never;
	}>
>(...checkFuncs: CheckFuncs): (b: unknown) => b is UnionToCheck {
	return function(x: unknown): x is UnionToCheck {
		let ret = false;
		for(const checkFunc of checkFuncs) {
			ret = ret || checkFunc(x);
		}
		return ret;
	}
}

function _checktype_intersection_factory<
	CheckFuncs extends ((a: unknown) => boolean)[],
	IntersectionToCheck = IntersectionOf<{
		[K in keyof CheckFuncs]: CheckFuncs[K] extends (c: unknown) => boolean ? GuardType<CheckFuncs[K]> : never;
	}>
>(...checkFuncs: CheckFuncs): (b: unknown) => b is IntersectionToCheck {
	return function(x: unknown): x is IntersectionToCheck {
		let ret = true;
		for(const checkFunc of checkFuncs) {
			ret = ret && checkFunc(x);
		}
		return ret;
	}
}

// TODO should add a stricter 'has ONLY these keys' version of this
function _checktype_hasallkeys_factory<
	IDK extends {
		required?: {
			[_: string]: (a: unknown) => boolean;
		},
		optional?: {
			[_: string]: (a: unknown) => boolean;
		}
	},
	// this will show up in the type system as { foo: bar } & { baz?: qux },
	//  so we do an extra step of iterating over it again, which turns it into
	//  the more readable form of { foo: bar, baz?: qux }
	_TypeToCheck_Intermediary = (
		IDK['required'] extends Exclude<IDK['required'], undefined> ? {
			[K in keyof IDK['required']]: GuardType<IDK['required'][K]>;
		} : {}
	) & (
		IDK['optional'] extends Exclude<IDK['optional'], undefined> ? {
			[K in keyof IDK['optional']]?: GuardType<IDK['optional'][K]>;
		} : {}
	),
	TypeToCheck = {
		[K in keyof _TypeToCheck_Intermediary]: _TypeToCheck_Intermediary[K]
	}
> (
	idk: IDK
): (a: unknown) => a is TypeToCheck {
	return (x: unknown): x is TypeToCheck => {
		if(!_checktype_object(x)) return false;
		let ret = true;
		if(idk.required !== undefined) {
			for(const key in idk.required) {
				ret = ret && (key in x) && idk.required[key](x[key as keyof typeof x]);
			}
		}
		if(idk.optional !== undefined) {
			for(const key in idk.optional) {
				if(key in x) ret &&= idk.optional[key](x[key as keyof typeof x]);
			}
		}
		return ret;
	}
}

function _checktype_arrayof_factory<
	Guard extends (a: unknown) => boolean
> (
	guard: Guard
): (a: unknown) => a is GuardType<Guard>[] {
	return (x: unknown): x is GuardType<Guard>[] => {
		if(!_checktype_array(x)) return false;
		let ret = true;
		for(const y of x) {
			ret = ret && guard(y);
		}
		return ret;
	}
}

function _checktype_tuple_factory<
	CheckFuncs extends ((a: unknown) => boolean)[],
	TypeToCheck = {
		[K in keyof CheckFuncs]: CheckFuncs[K] extends (a: unknown) => boolean ? GuardType<CheckFuncs[K]>: never;
	}
> (...checkFuncs: CheckFuncs): (a: unknown) => a is TypeToCheck {
	return (x: unknown): x is TypeToCheck => {
		if(!_checktype_array(x)) return false;
		if(x.length !== checkFuncs.length) return false;
		for(let i = 0; i < checkFuncs.length; i++) {
			if(!checkFuncs[i](x[i])) return false;
		}
		return true;
	}
}

function _checktype_strictlyoneof_factory<
	CheckFuncs extends ((a: unknown) => boolean)[],
	TypeToCheck = UnionOf<{
		[K in keyof CheckFuncs]: CheckFuncs[K] extends (c: unknown) => boolean ? GuardType<CheckFuncs[K]> : never;
	}>
> (...checkFuncs: CheckFuncs): (a: unknown) => a is TypeToCheck {
	return (x: unknown): x is TypeToCheck => {
		let numMatches = 0;
		for(const checkFunc of checkFuncs) {
			if(checkFunc(x)) numMatches++;
		}
		return numMatches === 1;
	}
}
