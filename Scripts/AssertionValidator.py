"""
Library of helper functions that can be used to validate a payload against a package
description.
"""
import operator


def or_op(lhs, rhs):
    """
    Logical OR operation

    :param lhs: lefthand side of the operation
    :param rhs: righthand side of the operation

    :return: the result of the logical OR
    :rtype: Boolean
    """
    return lhs or rhs


def and_op(lhs, rhs):
    """
    Logical AND operation

    :param lhs: lefthand side of the operation
    :param rhs: righthand side of the operation

    :return: the result of the logical AND
    :rtype: Boolean
    """
    return lhs and rhs


def in_op(lhs, rhs):
    """
    In operation

    :param lhs: value to check inclusion of
    :param rhs: set to be included in

    :return: whether the lhs is in set rhs
    :rtype: Boolean
    """
    return lhs in rhs


def notin_op(lhs, rhs):
    """
    Not in operation

    :param lhs: value to check exclusion of
    :param rhs: set to be excluded from

    :return: whether the lhs is not in set rhs
    :rtype: Boolean
    """
    return lhs not in rhs


# Mapping of operator string to operator function
operator_map = {
    "or": or_op,
    "and": and_op,
    "in": in_op,
    "not in": notin_op,
    ">": operator.gt,
    ">=": operator.ge,
    "<": operator.lt,
    "<=": operator.le,
    "!=": operator.ne,
    "==": operator.eq,
}


def check_assertion(payload, assertion):
    """
    Verifies that a payload doesn't violate an assertion

    :param payload: the payload object
    :param assertion: an assertion string

    :return: True if there are no violations, False otherwise
    :rtype: Boolean
    """
    # Verify the operator is valid
    if "operator" not in assertion or assertion["operator"] not in operator_map:
        return False

    # Get the value from the LHS
    if "operator" in assertion["lhs"]:
        lhs = check_assertion(payload, assertion["lhs"])
    else:
        lhs = assertion["lhs"]["value"]
        if assertion["lhs"]["type"] == "payload_key":
            lhs = payload["arguments"][lhs]

    # Get the value from the RHS
    if "operator" in assertion["rhs"]:
        rhs = check_assertion(payload, assertion["rhs"])
    else:
        rhs = assertion["rhs"]["value"]
        if assertion["rhs"]["type"] == "payload_key":
            rhs = payload["arguments"][rhs]

    # Call the operator on the LHS & RHS
    return operator_map[assertion["operator"]](lhs, rhs)


def check_assertions(payload, assertions):
    """
    Verifies that the payload doesn't violate any of the assertions.

    :param payload: the payload object
    :param assertions: a list of assertion objects from the system description

    :return: Tuple with the first element being whether the assertion is valid or not
             and the second being the failing assertion or None.
    :rtype: (Boolean, dict)
    """
    for assertion in assertions:
        if not check_assertion(payload, assertion["assertion"]):
            return False, assertion
    return True, None
