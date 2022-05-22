#ifndef WEBSERVER_RESPONSEUTILITIES_HPP
#define WEBSERVER_RESPONSEUTILITIES_HPP

namespace http
{
	enum StatusCode {
		Continue = 100,
		SwitchingProtocols,
		Ok = 200,
		Created,
		Accepted,
		NonAuthoritativeInformation,
		NoContent,
		ResetContent,
		PartialContent,
		MultipleChoices = 300,
		MovedPermanently,
		Found,
		SeeOther,
		NotModified,
		UseProxy,
		TemporaryRedirect,
		BadRequest = 400,
		Unauthorized,
		PaymentRequired,
		Forbidden,
		NotFound,
		MethodNotAllowed,
		NotAcceptable,
		ProxyAuthenticationRequired,
		RequestTimeOut,
		Conflict,
		Gone,
		LengthRequired,
		PreconditionFailed,
		RequestEntityTooLarge,
		RequestURITooLarge,
		UnsupportedMediaType,
		RequestedRangeNotSatisfiable,
		ExpectationFailed,
		InternalServerError = 500,
		NotImplemented,
		BadGateway,
		ServiceUnavailable,
		GatewayTimeOut,
		HTTPVersionNotSupported
	};
	enum ConnectionStatus {
		keepAlive,
		close
	};
	enum ContentType {
		text,
		image,
		audio,
		video,
	};
	enum ContentSubtype {

	};
}

#endif //WEBSERVER_RESPONSEUTILITIES_HPP
