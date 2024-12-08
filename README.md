# openapi-java-client

Transaction Service

- API version: 1.0.0

- Build date: 2024-04-25T19:22:30.933843-07:00[America/Los_Angeles]

- Generator version: 7.5.0

#### Notes

* Allows for the loading of funds to a user's account.
* Handles the authorization of a transaction based on a user's balance.
* Requires realtime synchronous calculation of balance on request.



*Automatically generated by the [OpenAPI Generator](https://openapi-generator.tech)*

## Requirements

Building the API client library requires:

1. Java 1.8+
2. Maven/Gradle

## Installation

To install the API client library to your local Maven repository, simply execute:

```shell
mvn clean install
```

To deploy it to a remote Maven repository instead, configure the settings of the repository and execute:

```shell
mvn clean deploy
```

Refer to the [OSSRH Guide](http://central.sonatype.org/pages/ossrh-guide.html) for more information.

### Maven users

Add this dependency to your project's POM:

```xml
<dependency>
  <groupId>org.openapitools</groupId>
  <artifactId>openapi-java-client</artifactId>
  <version>1.0.0</version>
  <scope>compile</scope>
</dependency>
```

### Gradle users

Add this dependency to your project's build file:

```groovy
  repositories {
    mavenCentral()     // Needed if the 'openapi-java-client' jar has been published to maven central.
    mavenLocal()       // Needed if the 'openapi-java-client' jar has been published to the local maven repo.
  }

  dependencies {
     implementation "org.openapitools:openapi-java-client:1.0.0"
  }
```

### Others

At first generate the JAR by executing:

```shell
mvn clean package
```

Then manually install the following JARs:

- `target/openapi-java-client-1.0.0.jar`
- `target/lib/*.jar`

## Getting Started

Please follow the [installation](#installation) instruction and execute the following Java code:

```java

import org.openapitools.client.*;
import org.openapitools.client.auth.*;
import org.openapitools.client.model.*;
import org.openapitools.client.api.DefaultApi;

public class DefaultApiExample {

    public static void main(String[] args) {
        ApiClient defaultClient = Configuration.getDefaultApiClient();
        defaultClient.setBasePath("http://localhost");
        
        DefaultApi apiInstance = new DefaultApi(defaultClient);
        String messageId = "messageId_example"; // String | Unique identifier for a message
        AuthorizationRequest authorizationRequest = new AuthorizationRequest(); // AuthorizationRequest | An authorization request message that needs to be decisioned.
        try {
            AuthorizationResponse result = apiInstance.authorizationMessageIdPut(messageId, authorizationRequest);
            System.out.println(result);
        } catch (ApiException e) {
            System.err.println("Exception when calling DefaultApi#authorizationMessageIdPut");
            System.err.println("Status code: " + e.getCode());
            System.err.println("Reason: " + e.getResponseBody());
            System.err.println("Response headers: " + e.getResponseHeaders());
            e.printStackTrace();
        }
    }
}

```

## Documentation for API Endpoints

All URIs are relative to *http://localhost*

Class | Method | HTTP request | Description
------------ | ------------- | ------------- | -------------
*DefaultApi* | [**authorizationMessageIdPut**](docs/DefaultApi.md#authorizationMessageIdPut) | **PUT** /authorization/{messageId} | Removes funds from a user&#39;s account if sufficient funds are available.
*DefaultApi* | [**loadMessageIdPut**](docs/DefaultApi.md#loadMessageIdPut) | **PUT** /load/{messageId} | Adds funds to a user&#39;s account.
*DefaultApi* | [**pingGet**](docs/DefaultApi.md#pingGet) | **GET** /ping | Tests the availability of the service and returns the current server time.


## Documentation for Models

 - [Amount](docs/Amount.md)
 - [AuthorizationRequest](docs/AuthorizationRequest.md)
 - [AuthorizationResponse](docs/AuthorizationResponse.md)
 - [DebitCredit](docs/DebitCredit.md)
 - [Error](docs/Error.md)
 - [LoadRequest](docs/LoadRequest.md)
 - [LoadResponse](docs/LoadResponse.md)
 - [Ping](docs/Ping.md)
 - [ResponseCode](docs/ResponseCode.md)


<a id="documentation-for-authorization"></a>
## Documentation for Authorization

Endpoints do not require authorization.


## Recommendation

It's recommended to create an instance of `ApiClient` per thread in a multithreaded environment to avoid any potential issues.

## Author



# threading
